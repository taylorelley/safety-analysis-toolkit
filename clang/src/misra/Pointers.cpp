// SPDX-License-Identifier: Apache-2.0
// Rule 8.13 and rules 18.x-19.x: pointers, arrays and overlapping storage.
// Object identity is only tracked through local, single-assignment pointers.
#include "Misra.h"
#include "clang/AST/RecursiveASTVisitor.h"

using namespace clang;
using namespace clang::ast_matchers;

namespace safety::misra {
namespace {
// ---- Rule 8.13 ----------------------------------------------------------------
bool constPointee(QualType T) {
  const auto *P = T->getAs<PointerType>();
  return P && P->getPointeeType().isConstQualified();
}
bool mayModifyThrough(ASTContext &C, const Expr *Pointer);
// Whether the lvalue E (an object reached through the pointer) is modified.
bool lvalueModified(ASTContext &C, const Expr *E) {
  const Stmt *Child = E;
  for (const Stmt *P = parentStmt(C, E); P; Child = P, P = parentStmt(C, P)) {
    if (isa<ParenExpr>(P)) continue;
    if (const auto *M = dyn_cast<MemberExpr>(P); M && !M->isArrow() && M->getBase() == Child) continue;
    if (const auto *A = dyn_cast<ArraySubscriptExpr>(P); A && A->getBase() == Child) continue;
    if (const auto *B = dyn_cast<BinaryOperator>(P)) return B->isAssignmentOp() && B->getLHS() == Child;
    if (const auto *U = dyn_cast<UnaryOperator>(P)) {
      if (U->isIncrementDecrementOp()) return true;
      if (U->getOpcode() == UO_AddrOf) return mayModifyThrough(C, U);
      return false;
    }
    if (const auto *Cast = dyn_cast<ImplicitCastExpr>(P)) {
      if (Cast->getCastKind() == CK_ArrayToPointerDecay) return mayModifyThrough(C, Cast);
      if (Cast->getCastKind() == CK_LValueToRValue) return false;
      continue;
    }
    return false;
  }
  return false;
}
// Whether the pointer value Pointer is used to modify, or escapes to a
// pointer that may modify, the object it points to.
bool mayModifyThrough(ASTContext &C, const Expr *Pointer) {
  const Stmt *Child = Pointer;
  for (const Stmt *P = parentStmt(C, Pointer); P; Child = P, P = parentStmt(C, P)) {
    if (isa<ParenExpr>(P)) continue;
    if (const auto *Cast = dyn_cast<ImplicitCastExpr>(P)) {
      if (Cast->getType()->isPointerType() && constPointee(Cast->getType())) return false;
      continue;
    }
    if (const auto *U = dyn_cast<UnaryOperator>(P)) {
      if (U->getOpcode() == UO_Deref) return lvalueModified(C, U);
      return U->getOpcode() == UO_AddrOf;
    }
    if (const auto *A = dyn_cast<ArraySubscriptExpr>(P)) return A->getBase() == Child && lvalueModified(C, A);
    if (const auto *M = dyn_cast<MemberExpr>(P)) return M->isArrow() && lvalueModified(C, M);
    if (const auto *B = dyn_cast<BinaryOperator>(P)) {
      if (B->isAdditiveOp() && B->getType()->isPointerType()) continue;
      if (B->getOpcode() == BO_Assign && B->getRHS() == Child) return !constPointee(B->getLHS()->getType());
      return false;
    }
    if (isa<ConditionalOperator>(P)) continue;
    if (const auto *Call = dyn_cast<CallExpr>(P)) {
      if (Call->getCallee() == Child) return false;
      return !constPointee(cast<Expr>(Child)->getType());
    }
    if (const auto *Cast = dyn_cast<CStyleCastExpr>(P))
      return !(Cast->getType()->isVoidType() || constPointee(Cast->getType()));
    if (isa<ReturnStmt>(P)) return !constPointee(cast<Expr>(Child)->getType());
    return false;
  }
  if (const auto *V = dyn_cast_or_null<VarDecl>(parentDecl(C, Child))) return !constPointee(V->getType());
  return false;
}
class ParamUses : public RecursiveASTVisitor<ParamUses> {
public:
  bool VisitDeclRefExpr(DeclRefExpr *E) {
    if (const auto *P = dyn_cast<ParmVarDecl>(E->getDecl())) Uses[P].push_back(E);
    return true;
  }
  std::map<const ParmVarDecl *, std::vector<const DeclRefExpr *>> Uses;
};

// ---- object identity (Rules 18.2, 18.3, 19.1) --------------------------------------
struct Object {
  const VarDecl *Var = nullptr;
  bool Parameter = false;
  std::optional<int64_t> Offset; // element offset when known
};
bool reassigned(const Stmt *Body, const VarDecl *V) {
  if (!Body) return false;
  if (const auto *B = dyn_cast<BinaryOperator>(Body); B && B->isAssignmentOp())
    if (const auto *D = dyn_cast<DeclRefExpr>(stripAll(B->getLHS())); D && D->getDecl() == V) return true;
  if (const auto *U = dyn_cast<UnaryOperator>(Body); U && (U->isIncrementDecrementOp() || U->getOpcode() == UO_AddrOf))
    if (const auto *D = dyn_cast<DeclRefExpr>(stripAll(U->getSubExpr())); D && D->getDecl() == V) return true;
  for (const Stmt *Child : Body->children())
    if (reassigned(Child, V)) return true;
  return false;
}
Object objectOf(ASTContext &C, const Expr *E, const Stmt *Body, int Depth = 0) {
  if (!E || Depth > 8) return {};
  E = stripAll(E);
  if (const auto *D = dyn_cast<DeclRefExpr>(E)) {
    const auto *V = dyn_cast<VarDecl>(D->getDecl());
    if (!V) return {};
    if (V->getType()->isArrayType()) return {V, false, 0};
    if (!V->getType()->isPointerType()) return {V, false, 0};
    if (isa<ParmVarDecl>(V)) return reassigned(Body, V) ? Object{} : Object{V, true, std::nullopt};
    if (V->hasLocalStorage() && V->getInit() && !reassigned(Body, V)) return objectOf(C, V->getInit(), Body, Depth + 1);
    return {};
  }
  if (const auto *U = dyn_cast<UnaryOperator>(E); U && U->getOpcode() == UO_AddrOf) {
    const Expr *Sub = stripAll(U->getSubExpr());
    if (const auto *A = dyn_cast<ArraySubscriptExpr>(Sub)) {
      Object O = objectOf(C, A->getBase(), Body, Depth + 1);
      auto Index = integerConstant(A->getIdx(), C);
      O.Offset = (O.Offset && Index) ? std::optional<int64_t>(*O.Offset + Index->getExtValue()) : std::nullopt;
      return O;
    }
    if (const auto *M = dyn_cast<MemberExpr>(Sub); M && !M->isArrow()) return {namedVariable(M), false, std::nullopt};
    if (const auto *D = dyn_cast<DeclRefExpr>(Sub)) return {dyn_cast<VarDecl>(D->getDecl()), false, 0};
    return {};
  }
  if (const auto *B = dyn_cast<BinaryOperator>(E); B && B->isAdditiveOp() && B->getType()->isPointerType()) {
    const Expr *Ptr = B->getLHS()->getType()->isPointerType() ? B->getLHS() : B->getRHS();
    const Expr *Int = Ptr == B->getLHS() ? B->getRHS() : B->getLHS();
    Object O = objectOf(C, Ptr, Body, Depth + 1);
    auto K = integerConstant(Int, C);
    O.Offset = (O.Offset && K) ? std::optional<int64_t>(*O.Offset + (B->getOpcode() == BO_Sub ? -1 : 1) * K->getExtValue())
                               : std::nullopt;
    return O;
  }
  return {};
}
// Two pointers that demonstrably address different objects.
bool differentObjects(const Object &A, const Object &B) {
  if (!A.Var || !B.Var || A.Var == B.Var) return false;
  if (A.Parameter && B.Parameter) return false;
  if (A.Parameter || B.Parameter) {
    const Object &Local = A.Parameter ? B : A;
    return Local.Var->hasLocalStorage();
  }
  return true;
}
const Stmt *bodyOf(ASTContext &C, const Stmt *S) {
  auto Parents = C.getParents(*S);
  while (!Parents.empty()) {
    if (const auto *F = Parents[0].get<FunctionDecl>()) return F->getBody();
    if (const auto *P = Parents[0].get<Stmt>()) Parents = C.getParents(*P);
    else if (const auto *D = Parents[0].get<Decl>()) Parents = C.getParents(*D);
    else break;
  }
  return nullptr;
}

// ---- Rule 18.5 ----------------------------------------------------------------
unsigned pointerRun(QualType T, unsigned Run = 0) {
  if (T.isNull()) return Run;
  T = T.getCanonicalType();
  if (const auto *P = T->getAs<PointerType>()) return pointerRun(P->getPointeeType(), Run + 1);
  if (const auto *A = T->getAsArrayTypeUnsafe()) return std::max(Run, pointerRun(A->getElementType(), 0));
  if (const auto *F = T->getAs<FunctionType>()) {
    unsigned Max = std::max(Run, pointerRun(F->getReturnType(), 0));
    if (const auto *Proto = dyn_cast<FunctionProtoType>(F))
      for (QualType Param : Proto->getParamTypes()) Max = std::max(Max, pointerRun(Param, 0));
    return Max;
  }
  return Run;
}

// ---- Rule 18.6 ----------------------------------------------------------------
const VarDecl *automaticAddress(const Expr *E) {
  E = stripAll(E);
  if (const auto *U = dyn_cast<UnaryOperator>(E); U && U->getOpcode() == UO_AddrOf) {
    const Expr *Sub = stripAll(U->getSubExpr());
    while (const auto *A = dyn_cast<ArraySubscriptExpr>(Sub)) Sub = stripAll(A->getBase());
    const VarDecl *V = namedVariable(Sub);
    return V && V->hasLocalStorage() ? V : nullptr;
  }
  if (const auto *D = dyn_cast<DeclRefExpr>(E)) {
    const auto *V = dyn_cast<VarDecl>(D->getDecl());
    if (V && V->hasLocalStorage() && V->getType()->isArrayType() && !isa<ParmVarDecl>(V)) return V;
  }
  return nullptr;
}
bool persists(const Expr *Target) {
  Target = stripAll(Target);
  if (const VarDecl *V = namedVariable(Target)) return !V->hasLocalStorage();
  if (const auto *U = dyn_cast<UnaryOperator>(Target); U && U->getOpcode() == UO_Deref) return true;
  if (const auto *M = dyn_cast<MemberExpr>(Target); M && M->isArrow()) return true;
  return false;
}

// ---- Rule 19.1 ----------------------------------------------------------------
std::optional<int64_t> byteOffset(ASTContext &C, const Object &O, const Expr *E) {
  if (!O.Offset) return std::nullopt;
  QualType Element = stripAll(E)->getType();
  if (const auto *P = Element->getAs<PointerType>()) Element = P->getPointeeType();
  if (Element->isIncompleteType() || Element->isVoidType()) return std::nullopt;
  return *O.Offset * static_cast<int64_t>(C.getTypeSizeInChars(Element).getQuantity());
}
} // namespace

void addRulesPointers(Table &T) {
  T.push_back({"misra-c2012-rule-8-13", [] {
                 return matcherSpec(functionDecl(isDefinition()).bind("f"), [](Rule &R, const Result &Res) {
                   const auto *F = Res.Nodes.getNodeAs<FunctionDecl>("f");
                   if (!isUserDecl(F)) return;
                   ParamUses V;
                   V.TraverseStmt(F->getBody());
                   for (const ParmVarDecl *P : F->parameters()) {
                     const auto *Ptr = P->getType()->getAs<PointerType>();
                     if (!Ptr || Ptr->getPointeeType().isConstQualified() || Ptr->getPointeeType()->isFunctionType()) continue;
                     auto It = V.Uses.find(P);
                     if (It == V.Uses.end()) continue;
                     bool Modifies = false;
                     for (const DeclRefExpr *Use : It->second) Modifies |= mayModifyThrough(*Res.Context, Use);
                     if (!Modifies) R.report(P->getLocation(), "pointer parameter could point to a const-qualified type");
                   }
                 });
               }});
  T.push_back({"misra-c2012-rule-18-1", [] {
                 Spec S;
                 S.Matchers = [](MatchFinder &F, Rule &R) {
                   F.addMatcher(arraySubscriptExpr().bind("a"), &R);
                   F.addMatcher(binaryOperator(hasAnyOperatorName("+", "-"), hasType(pointerType())).bind("b"), &R);
                 };
                 S.Check = [](Rule &R, const Result &Res) {
                   ASTContext &C = *Res.Context;
                   const Expr *Base = nullptr, *Index = nullptr;
                   bool Negate = false, Dereferenced = true;
                   const Expr *Node = nullptr;
                   if (const auto *A = Res.Nodes.getNodeAs<ArraySubscriptExpr>("a")) {
                     Base = A->getBase(), Index = A->getIdx(), Node = A;
                     const Stmt *P = parentStmt(C, A);
                     while (isa_and_nonnull<ParenExpr>(P)) P = parentStmt(C, P);
                     if (const auto *U = dyn_cast_or_null<UnaryOperator>(P); U && U->getOpcode() == UO_AddrOf) Dereferenced = false;
                   }
                   if (const auto *B = Res.Nodes.getNodeAs<BinaryOperator>("b")) {
                     Base = B->getLHS()->getType()->isPointerType() ? B->getLHS() : B->getRHS();
                     Index = Base == B->getLHS() ? B->getRHS() : B->getLHS();
                     Negate = B->getOpcode() == BO_Sub, Dereferenced = false, Node = B;
                   }
                   if (!isUserLocation(C.getSourceManager(), Node->getExprLoc())) return;
                   const auto *Array = C.getAsConstantArrayType(stripAll(Base)->getType());
                   auto K = integerConstant(Index, C);
                   if (!Array || !K) return;
                   int64_t I = Negate ? -K->getExtValue() : K->getExtValue();
                   int64_t N = static_cast<int64_t>(Array->getSize().getZExtValue());
                   if (I < 0 || I > N || (I == N && Dereferenced))
                     R.report(Node->getExprLoc(), "array element or pointer is outside the bounds of the array");
                 };
                 return S;
               }});
  auto SameObject = [](auto Operators, const char *Message) {
    return [Operators, Message] {
      return matcherSpec(binaryOperator(Operators, hasLHS(hasType(pointerType())), hasRHS(hasType(pointerType()))).bind("b"),
                         [Message](Rule &R, const Result &Res) {
                           const auto *B = Res.Nodes.getNodeAs<BinaryOperator>("b");
                           ASTContext &C = *Res.Context;
                           if (!isUserLocation(C.getSourceManager(), B->getOperatorLoc())) return;
                           const Stmt *Body = bodyOf(C, B);
                           if (differentObjects(objectOf(C, B->getLHS(), Body), objectOf(C, B->getRHS(), Body)))
                             R.report(B->getOperatorLoc(), Message);
                         });
    };
  };
  T.push_back({"misra-c2012-rule-18-2", SameObject(hasOperatorName("-"), "pointer subtraction between different objects")});
  T.push_back({"misra-c2012-rule-18-3", SameObject(hasAnyOperatorName("<", ">", "<=", ">="), "relational comparison of pointers to different objects")});
  T.push_back({"misra-c2012-rule-18-4", [] {
                 return matcherSpec(binaryOperator(hasAnyOperatorName("+", "-", "+=", "-=")).bind("b"), [](Rule &R, const Result &Res) {
                   const auto *B = Res.Nodes.getNodeAs<BinaryOperator>("b");
                   bool L = B->getLHS()->getType()->isPointerType(), Rp = B->getRHS()->getType()->isPointerType();
                   if ((L || Rp) && !(L && Rp) && isUserLocation(*Res.SourceManager, B->getOperatorLoc()))
                     R.report(B->getOperatorLoc(), "arithmetic operator applied to an expression of pointer type");
                 });
               }});
  T.push_back({"misra-c2012-rule-18-5", [] {
                 return matcherSpec(decl(anyOf(declaratorDecl(), typedefNameDecl())).bind("d"), [](Rule &R, const Result &Res) {
                   const auto *D = Res.Nodes.getNodeAs<Decl>("d");
                   if (!isUserDecl(D)) return;
                   QualType T;
                   if (const auto *F = dyn_cast<FunctionDecl>(D)) T = F->getReturnType();
                   else if (const auto *Td = dyn_cast<TypedefNameDecl>(D)) T = Td->getUnderlyingType();
                   else T = cast<DeclaratorDecl>(D)->getType();
                   if (pointerRun(T) > 2)
                     R.report(cast<NamedDecl>(D)->getLocation(), "declaration has more than two levels of pointer nesting");
                 });
               }});
  T.push_back({"misra-c2012-rule-18-6", [] {
                 Spec S;
                 S.Matchers = [](MatchFinder &F, Rule &R) {
                   F.addMatcher(returnStmt(hasReturnValue(expr())).bind("r"), &R);
                   F.addMatcher(binaryOperator(hasOperatorName("=")).bind("a"), &R);
                 };
                 S.Check = [](Rule &R, const Result &Res) {
                   if (const auto *Ret = Res.Nodes.getNodeAs<ReturnStmt>("r")) {
                     if (isUserLocation(*Res.SourceManager, Ret->getReturnLoc()) && automaticAddress(Ret->getRetValue()))
                       R.report(Ret->getReturnLoc(), "address of an automatic object outlives the object");
                     return;
                   }
                   const auto *B = Res.Nodes.getNodeAs<BinaryOperator>("a");
                   if (isUserLocation(*Res.SourceManager, B->getOperatorLoc()) && automaticAddress(B->getRHS()) &&
                       persists(B->getLHS()))
                     R.report(B->getOperatorLoc(), "address of an automatic object is stored in an object that outlives it");
                 };
                 return S;
               }});
  T.push_back({"misra-c2012-rule-18-7", [] {
                 return matcherSpec(fieldDecl(hasType(incompleteArrayType())).bind("f"), [](Rule &R, const Result &Res) {
                   const auto *F = Res.Nodes.getNodeAs<FieldDecl>("f");
                   if (isUserDecl(F)) R.report(F->getLocation(), "flexible array member declared");
                 });
               }});
  T.push_back({"misra-c2012-rule-18-8", [] {
                 return unitSpec([](Rule &R, ASTContext &C) {
                   struct V : RecursiveASTVisitor<V> {
                     Rule &R;
                     ASTContext &C;
                     V(Rule &R, ASTContext &C) : R(R), C(C) {}
                     bool VisitVariableArrayTypeLoc(VariableArrayTypeLoc TL) {
                       if (isUserLocation(C.getSourceManager(), TL.getBeginLoc()))
                         R.report(TL.getLBracketLoc(), "variable-length array type used");
                       return true;
                     }
                   } Walk(R, C);
                   Walk.TraverseDecl(C.getTranslationUnitDecl());
                 });
               }});
  T.push_back({"misra-c2012-rule-19-1", [] {
                 Spec S;
                 S.Matchers = [](MatchFinder &F, Rule &R) {
                   F.addMatcher(binaryOperator(hasOperatorName("=")).bind("a"), &R);
                   F.addMatcher(callExpr(callee(functionDecl(hasAnyName("memcpy", "strcpy", "strncpy", "strcat", "strncat",
                                                                          "wmemcpy", "wcscpy", "wcsncpy"))))
                                    .bind("c"),
                                &R);
                 };
                 S.Check = [](Rule &R, const Result &Res) {
                   ASTContext &C = *Res.Context;
                   if (const auto *B = Res.Nodes.getNodeAs<BinaryOperator>("a")) {
                     const auto *L = dyn_cast<MemberExpr>(stripAll(B->getLHS()));
                     const auto *Rm = dyn_cast<MemberExpr>(stripAll(B->getRHS()));
                     if (!L || !Rm || L->isArrow() || Rm->isArrow() || !isUserLocation(C.getSourceManager(), B->getOperatorLoc()))
                       return;
                     const RecordDecl *Union = L->getBase()->getType()->getAsRecordDecl();
                     if (Union && Union->isUnion() && objectKey(L->getBase()) == objectKey(Rm->getBase()) &&
                         !objectKey(L->getBase()).empty() && L->getMemberDecl() != Rm->getMemberDecl())
                       R.report(B->getOperatorLoc(), "object is assigned to an overlapping union member");
                     return;
                   }
                   const auto *Call = Res.Nodes.getNodeAs<CallExpr>("c");
                   if (Call->getNumArgs() < 2 || !isUserLocation(C.getSourceManager(), Call->getBeginLoc())) return;
                   const Stmt *Body = bodyOf(C, Call);
                   Object Dst = objectOf(C, Call->getArg(0), Body), Src = objectOf(C, Call->getArg(1), Body);
                   if (!Dst.Var || Dst.Var != Src.Var) return;
                   auto D = byteOffset(C, Dst, Call->getArg(0)), Sr = byteOffset(C, Src, Call->getArg(1));
                   auto Size = Call->getNumArgs() > 2 ? integerConstant(Call->getArg(2), C) : std::nullopt;
                   if (D && Sr && Size) {
                     int64_t N = Size->getExtValue();
                     if (*D + N <= *Sr || *Sr + N <= *D) return;
                   }
                   R.report(Call->getBeginLoc(), "source and destination of a copy may overlap");
                 };
                 return S;
               }});
  T.push_back({"misra-c2012-rule-19-2", [] {
                 Spec S;
                 S.Matchers = [](MatchFinder &F, Rule &R) { F.addMatcher(recordDecl(isUnion()).bind("u"), &R); };
                 S.Check = [](Rule &R, const Result &Res) {
                   const auto *U = Res.Nodes.getNodeAs<RecordDecl>("u");
                   if (isUserDecl(U)) R.report(U->getBeginLoc(), "union keyword used");
                 };
                 return S;
               }});
}
} // namespace safety::misra
