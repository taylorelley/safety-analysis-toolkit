// SPDX-License-Identifier: Apache-2.0
// Rules 10.x (essential type model) and 11.x (pointer type conversions).
#include "Misra.h"
#include "clang/AST/RecursiveASTVisitor.h"

using namespace clang;
using namespace clang::ast_matchers;

namespace safety::misra {
namespace {
bool in(Cat C, std::initializer_list<Cat> Set) { return std::find(Set.begin(), Set.end(), C) != Set.end(); }
bool integerCat(Cat C) { return C == Cat::Signed || C == Cat::Unsigned; }
constexpr std::initializer_list<Cat> NotBitwise = {Cat::Boolean, Cat::Character, Cat::Enum, Cat::Signed, Cat::Floating};

// ---- Rule 10.1 -------------------------------------------------------------
class OperandVisitor : public RecursiveASTVisitor<OperandVisitor> {
public:
  OperandVisitor(Rule &R, ASTContext &C) : R(R), C(C) {}
  void check(const Expr *Operand, SourceLocation Loc, llvm::StringRef Op, std::initializer_list<Cat> Bad,
             bool BooleanOnly = false) {
    if (!isUserLocation(C.getSourceManager(), Loc)) return;
    EType T = essentialType(Operand, C);
    if (!T.arithmetic()) return;
    if (BooleanOnly ? T.C != Cat::Boolean : in(T.C, Bad))
      R.report(Loc, (llvm::Twine("operand of essentially ") + categoryName(T.C) + " type is inappropriate for '" + Op + "'").str());
  }
  bool VisitBinaryOperator(BinaryOperator *B) {
    BinaryOperatorKind Op = B->isCompoundAssignmentOp() ? BinaryOperator::getOpForCompoundAssignment(B->getOpcode())
                                                        : B->getOpcode();
    llvm::StringRef Name = B->getOpcodeStr();
    SourceLocation L = B->getOperatorLoc();
    auto Both = [&](std::initializer_list<Cat> Bad) {
      check(B->getLHS(), L, Name, Bad);
      check(B->getRHS(), L, Name, Bad);
    };
    switch (Op) {
    case BO_Add:
    case BO_Sub: Both({Cat::Boolean, Cat::Enum}); break;
    case BO_Mul:
    case BO_Div: Both({Cat::Boolean, Cat::Character, Cat::Enum}); break;
    case BO_Rem: Both({Cat::Boolean, Cat::Character, Cat::Enum, Cat::Floating}); break;
    case BO_LT:
    case BO_GT:
    case BO_LE:
    case BO_GE: Both({Cat::Boolean}); break;
    case BO_LAnd:
    case BO_LOr:
      check(B->getLHS(), L, Name, {}, true);
      check(B->getRHS(), L, Name, {}, true);
      break;
    case BO_Shl:
    case BO_Shr: {
      check(B->getLHS(), L, Name, NotBitwise);
      auto V = integerConstant(B->getRHS(), C);
      if (!(V && !V->isNegative() && essentialType(B->getRHS(), C).C == Cat::Signed)) check(B->getRHS(), L, Name, NotBitwise);
      break;
    }
    case BO_And:
    case BO_Or:
    case BO_Xor: Both(NotBitwise); break;
    default: break;
    }
    return true;
  }
  bool VisitUnaryOperator(UnaryOperator *U) {
    llvm::StringRef Name = UnaryOperator::getOpcodeStr(U->getOpcode());
    SourceLocation L = U->getOperatorLoc();
    switch (U->getOpcode()) {
    case UO_Plus: check(U->getSubExpr(), L, Name, {Cat::Boolean, Cat::Character, Cat::Enum}); break;
    case UO_Minus: check(U->getSubExpr(), L, Name, {Cat::Boolean, Cat::Character, Cat::Enum, Cat::Unsigned}); break;
    case UO_Not: check(U->getSubExpr(), L, Name, NotBitwise); break;
    case UO_LNot: check(U->getSubExpr(), L, Name, {}, true); break;
    case UO_PreInc:
    case UO_PreDec:
    case UO_PostInc:
    case UO_PostDec: check(U->getSubExpr(), L, Name, {Cat::Boolean, Cat::Enum}); break;
    default: break;
    }
    return true;
  }
  bool VisitArraySubscriptExpr(ArraySubscriptExpr *E) {
    check(E->getIdx(), E->getRBracketLoc(), "[]", {Cat::Boolean, Cat::Character, Cat::Floating});
    return true;
  }
  bool VisitConditionalOperator(ConditionalOperator *E) {
    check(E->getCond(), E->getQuestionLoc(), "?:", {}, true);
    return true;
  }

private:
  Rule &R;
  ASTContext &C;
};

// ---- Rules 10.3 / 10.6 -------------------------------------------------------
std::optional<std::string> narrowing(const Expr *Source, EType S, EType T, ASTContext &C) {
  if (!S.arithmetic() || !T.arithmetic()) return std::nullopt;
  if (!S.same(T)) {
    if (S.C == Cat::Signed && T.C == Cat::Unsigned)
      if (auto V = integerConstant(stripAll(Source), C); V && !V->isNegative() && V->getActiveBits() <= T.Bits)
        return std::nullopt;
    return (llvm::Twine("essentially ") + categoryName(S.C) + " value assigned to an object of essentially " +
            categoryName(T.C) + " type")
        .str();
  }
  if (S.Bits > T.Bits && S.C != Cat::Enum && S.C != Cat::Boolean)
    return std::string("value assigned to an object of narrower essential type");
  return std::nullopt;
}
bool zeroInitializer(ASTContext &C, const Expr *E) {
  const Stmt *P = parentStmt(C, E);
  while (isa_and_nonnull<ImplicitCastExpr>(P)) P = parentStmt(C, P);
  const auto *L = dyn_cast_or_null<InitListExpr>(P);
  if (!L) return false;
  const InitListExpr *Syntax = L->isSemanticForm() && L->getSyntacticForm() ? L->getSyntacticForm() : L;
  const auto *I = dyn_cast<IntegerLiteral>(stripAll(Syntax->getNumInits() == 1 ? Syntax->getInit(0) : nullptr));
  return I && I->getValue() == 0;
}
Spec rule103() {
  return unitSpec([](Rule &R, ASTContext &C) {
    forEachAssignment(C, [&](const Assignment &A) {
      if (zeroInitializer(C, A.Source)) return;
      if (auto Why = narrowing(A.Source, essentialType(A.Source, C), A.Target, C)) R.report(A.Loc, *Why);
    });
    struct V : RecursiveASTVisitor<V> {
      Rule &R;
      ASTContext &C;
      V(Rule &R, ASTContext &C) : R(R), C(C) {}
      bool VisitCompoundAssignOperator(CompoundAssignOperator *B) {
        if (!isUserLocation(C.getSourceManager(), B->getOperatorLoc()) || B->isShiftAssignOp()) return true;
        EType L = essentialType(B->getLHS(), C), Rt = essentialType(B->getRHS(), C), Result;
        BinaryOperatorKind Op = BinaryOperator::getOpForCompoundAssignment(B->getOpcode());
        if (L.C == Cat::Character && integerCat(Rt.C) && (Op == BO_Add || Op == BO_Sub)) Result = L;
        else if (L.same(Rt) && integerCat(L.C)) Result = L.Bits >= Rt.Bits ? L : Rt;
        else if (L.C == Cat::Floating && Rt.C == Cat::Floating) Result = L.Bits >= Rt.Bits ? L : Rt;
        else Result = essentialOfType(B->getComputationResultType(), C);
        if (auto Why = narrowing(nullptr, Result, L, C)) R.report(B->getOperatorLoc(), *Why);
        return true;
      }
    } Walk(R, C);
    Walk.TraverseDecl(C.getTranslationUnitDecl());
  });
}
Spec rule106() {
  return unitSpec([](Rule &R, ASTContext &C) {
    forEachAssignment(C, [&](const Assignment &A) {
      if (A.CaseLabel || !isComposite(A.Source, C)) return;
      EType S = essentialType(A.Source, C);
      if (S.same(A.Target) && A.Target.Bits > S.Bits)
        R.report(A.Loc, "composite expression assigned to an object of wider essential type");
    });
  });
}

// ---- Rules 10.2 / 10.4 / 10.7 ---------------------------------------------------
bool usualArithmetic(BinaryOperatorKind Op) {
  switch (Op) {
  case BO_Mul: case BO_Div: case BO_Rem: case BO_Add: case BO_Sub: case BO_LT: case BO_GT: case BO_LE:
  case BO_GE: case BO_EQ: case BO_NE: case BO_And: case BO_Xor: case BO_Or: return true;
  default: return false;
  }
}
struct Operands {
  const Expr *L, *R;
  BinaryOperatorKind Op;
  SourceLocation Loc;
  bool Compound = false;
};
void forEachArithmeticPair(ASTContext &C, std::function<void(const Operands &)> F) {
  struct V : RecursiveASTVisitor<V> {
    ASTContext &C;
    std::function<void(const Operands &)> F;
    bool VisitBinaryOperator(BinaryOperator *B) {
      BinaryOperatorKind Op = B->isCompoundAssignmentOp() ? BinaryOperator::getOpForCompoundAssignment(B->getOpcode())
                                                          : B->getOpcode();
      if (usualArithmetic(Op) && isUserLocation(C.getSourceManager(), B->getOperatorLoc()))
        F({B->getLHS(), B->getRHS(), Op, B->getOperatorLoc(), B->isCompoundAssignmentOp()});
      return true;
    }
    bool VisitConditionalOperator(ConditionalOperator *E) {
      if (isUserLocation(C.getSourceManager(), E->getColonLoc()))
        F({E->getTrueExpr(), E->getFalseExpr(), BO_Comma, E->getColonLoc()});
      return true;
    }
  } Walk{{}, C, std::move(F)};
  Walk.TraverseDecl(C.getTranslationUnitDecl());
}
Spec rule102() {
  return matcherSpec(binaryOperator(hasAnyOperatorName("+", "-", "+=", "-=")).bind("b"), [](Rule &R, const Result &Res) {
    const auto *B = Res.Nodes.getNodeAs<BinaryOperator>("b");
    ASTContext &C = *Res.Context;
    if (!isUserLocation(C.getSourceManager(), B->getOperatorLoc())) return;
    EType L = essentialType(B->getLHS(), C), Rt = essentialType(B->getRHS(), C);
    if (L.C != Cat::Character && Rt.C != Cat::Character) return;
    bool Add = B->getOpcode() == BO_Add || B->getOpcode() == BO_AddAssign;
    bool Ok = Add ? ((L.C == Cat::Character && integerCat(Rt.C)) || (Rt.C == Cat::Character && integerCat(L.C)))
                  : (L.C == Cat::Character && (integerCat(Rt.C) || Rt.C == Cat::Character));
    if (!Ok) R.report(B->getOperatorLoc(), "essentially character operand used inappropriately in addition or subtraction");
  });
}
Spec rule104() {
  return unitSpec([](Rule &R, ASTContext &C) {
    forEachArithmeticPair(C, [&](const Operands &P) {
      EType L = essentialType(P.L, C), Rt = essentialType(P.R, C);
      if (!L.arithmetic() || !Rt.arithmetic() || L.same(Rt)) return;
      // += keeps the character operand on the left, where the result is stored.
      if (P.Op == BO_Add &&
          ((L.C == Cat::Character && integerCat(Rt.C)) || (!P.Compound && Rt.C == Cat::Character && integerCat(L.C))))
        return;
      if (P.Op == BO_Sub && L.C == Cat::Character && integerCat(Rt.C)) return;
      R.report(P.Loc, (llvm::Twine("operands have different essential type categories (") + categoryName(L.C) + " and " +
                       categoryName(Rt.C) + ")")
                          .str());
    });
  });
}
Spec rule107() {
  return unitSpec([](Rule &R, ASTContext &C) {
    forEachArithmeticPair(C, [&](const Operands &P) {
      for (auto [Composite, Other] : {std::pair{P.L, P.R}, std::pair{P.R, P.L}}) {
        if (!isComposite(Composite, C)) continue;
        EType X = essentialType(Composite, C), Y = essentialType(Other, C);
        if (X.same(Y) && Y.Bits > X.Bits)
          R.report(P.Loc, "composite expression is implicitly converted to the wider essential type of the other operand");
      }
    });
  });
}

// ---- Rules 10.5 / 10.8 ------------------------------------------------------------
Spec rule105() {
  return matcherSpec(cStyleCastExpr().bind("c"), [](Rule &R, const Result &Res) {
    const auto *E = Res.Nodes.getNodeAs<CStyleCastExpr>("c");
    ASTContext &C = *Res.Context;
    if (!isUserLocation(C.getSourceManager(), E->getBeginLoc())) return;
    EType To = essentialOfType(E->getType(), C), From = essentialType(E->getSubExpr(), C);
    if (!To.arithmetic() || !From.arithmetic()) return;
    bool Avoid = false;
    switch (To.C) {
    case Cat::Boolean: {
      auto V = integerConstant(E->getSubExpr(), C);
      Avoid = From.C != Cat::Boolean && !(V && (*V == 0 || *V == 1));
      break;
    }
    case Cat::Character: Avoid = in(From.C, {Cat::Boolean, Cat::Floating}); break;
    case Cat::Enum: Avoid = !From.same(To); break;
    case Cat::Signed:
    case Cat::Unsigned: Avoid = From.C == Cat::Boolean; break;
    case Cat::Floating: Avoid = in(From.C, {Cat::Boolean, Cat::Character}); break;
    case Cat::None: break;
    }
    if (Avoid)
      R.report(E->getBeginLoc(), (llvm::Twine("cast from essentially ") + categoryName(From.C) + " to essentially " +
                                  categoryName(To.C) + " type is inappropriate")
                                     .str());
  });
}
Spec rule108() {
  return matcherSpec(cStyleCastExpr().bind("c"), [](Rule &R, const Result &Res) {
    const auto *E = Res.Nodes.getNodeAs<CStyleCastExpr>("c");
    ASTContext &C = *Res.Context;
    if (!isUserLocation(C.getSourceManager(), E->getBeginLoc()) || !isComposite(E->getSubExpr(), C)) return;
    EType To = essentialOfType(E->getType(), C), From = essentialType(E->getSubExpr(), C);
    if (!To.arithmetic() || !From.arithmetic()) return;
    if (!To.same(From)) R.report(E->getBeginLoc(), "composite expression cast to a different essential type category");
    else if (To.Bits > From.Bits) R.report(E->getBeginLoc(), "composite expression cast to a wider essential type");
  });
}

// ---- Rule 11.x ------------------------------------------------------------------
enum class Ptr { None, Function, Object, Incomplete, Void };
Ptr pointerKind(QualType T) {
  if (T.isNull() || !T->isPointerType()) return Ptr::None;
  QualType P = T->getPointeeType();
  if (P->isFunctionType()) return Ptr::Function;
  if (P->isVoidType()) return Ptr::Void;
  if (P->isIncompleteType()) return Ptr::Incomplete;
  return Ptr::Object;
}
struct Conversion {
  const CastExpr *E;
  QualType From, To;
  Ptr FromKind, ToKind;
  bool Explicit, NullConstant;
};
std::optional<Conversion> conversion(const CastExpr *E, ASTContext &C) {
  if (E->getCastKind() == CK_NoOp && !isa<ExplicitCastExpr>(E)) return std::nullopt;
  switch (E->getCastKind()) {
  case CK_LValueToRValue: case CK_ArrayToPointerDecay: case CK_FunctionToPointerDecay:
  case CK_ToVoid: case CK_BuiltinFnToFnPtr: case CK_LValueBitCast: return std::nullopt;
  default: break;
  }
  const Expr *Sub = E->getSubExpr();
  Conversion X{E, Sub->getType(), E->getType(), pointerKind(Sub->getType()), pointerKind(E->getType()),
               isa<ExplicitCastExpr>(E),
               Sub->isNullPointerConstant(C, Expr::NPC_ValueDependentIsNotNull) != Expr::NPCK_NotNull};
  if (!X.Explicit && E->getCastKind() == CK_PointerToBoolean) return std::nullopt;
  return X;
}
Spec castRule(std::function<std::optional<std::string>(const Conversion &, ASTContext &)> F) {
  return matcherSpec(castExpr().bind("c"), [F](Rule &R, const Result &Res) {
    const auto *E = Res.Nodes.getNodeAs<CastExpr>("c");
    SourceLocation Loc = E->getExprLoc();
    if (const auto *Cast = dyn_cast<CStyleCastExpr>(E)) Loc = Cast->getLParenLoc();
    if (!isUserLocation(*Res.SourceManager, Loc)) return;
    if (auto X = conversion(E, *Res.Context))
      if (auto Why = F(*X, *Res.Context)) R.report(Loc, *Why);
  });
}
bool characterPointee(QualType P) {
  P = P.getCanonicalType().getUnqualifiedType();
  return P->isCharType();
}
} // namespace

void addRulesTypes(Table &T) {
  T.push_back({"misra-c2012-rule-10-1", [] {
                 return unitSpec([](Rule &R, ASTContext &C) { OperandVisitor(R, C).TraverseDecl(C.getTranslationUnitDecl()); });
               }});
  T.push_back({"misra-c2012-rule-10-2", rule102});
  T.push_back({"misra-c2012-rule-10-3", rule103});
  T.push_back({"misra-c2012-rule-10-4", rule104});
  T.push_back({"misra-c2012-rule-10-5", rule105});
  T.push_back({"misra-c2012-rule-10-6", rule106});
  T.push_back({"misra-c2012-rule-10-7", rule107});
  T.push_back({"misra-c2012-rule-10-8", rule108});
  T.push_back({"misra-c2012-rule-11-1", [] {
                 return castRule([](const Conversion &X, ASTContext &C) -> std::optional<std::string> {
                   if (X.FromKind != Ptr::Function && X.ToKind != Ptr::Function) return std::nullopt;
                   if (X.NullConstant || X.To->isVoidType()) return std::nullopt;
                   if (X.FromKind == Ptr::Function && X.ToKind == Ptr::Function && C.typesAreCompatible(X.From, X.To))
                     return std::nullopt;
                   return "conversion between a function pointer and another type";
                 });
               }});
  T.push_back({"misra-c2012-rule-11-2", [] {
                 return castRule([](const Conversion &X, ASTContext &C) -> std::optional<std::string> {
                   if (X.FromKind != Ptr::Incomplete && X.ToKind != Ptr::Incomplete) return std::nullopt;
                   if (X.NullConstant || X.To->isVoidType()) return std::nullopt;
                   if (C.hasSameType(X.From.getCanonicalType(), X.To.getCanonicalType())) return std::nullopt;
                   return "conversion involving a pointer to an incomplete type";
                 });
               }});
  T.push_back({"misra-c2012-rule-11-3", [] {
                 return castRule([](const Conversion &X, ASTContext &C) -> std::optional<std::string> {
                   if (!X.Explicit || X.FromKind != Ptr::Object || X.ToKind != Ptr::Object) return std::nullopt;
                   QualType A = X.From->getPointeeType(), B = X.To->getPointeeType();
                   if (C.hasSameUnqualifiedType(A, B) || characterPointee(B)) return std::nullopt;
                   return "cast between pointers to different object types";
                 });
               }});
  T.push_back({"misra-c2012-rule-11-4", [] {
                 return castRule([](const Conversion &X, ASTContext &) -> std::optional<std::string> {
                   auto ObjectLike = [](Ptr P) { return P == Ptr::Object || P == Ptr::Incomplete; };
                   CastKind K = X.E->getCastKind();
                   if (K == CK_IntegralToPointer && ObjectLike(X.ToKind) && !X.NullConstant)
                     return "integer converted to a pointer to object";
                   if ((K == CK_PointerToIntegral || K == CK_PointerToBoolean) && ObjectLike(X.FromKind))
                     return "pointer to object converted to an integer type";
                   return std::nullopt;
                 });
               }});
  T.push_back({"misra-c2012-rule-11-5", [] {
                 return castRule([](const Conversion &X, ASTContext &) -> std::optional<std::string> {
                   if (X.FromKind == Ptr::Void && X.ToKind == Ptr::Object && !X.NullConstant)
                     return "pointer to void converted to a pointer to object";
                   return std::nullopt;
                 });
               }});
  T.push_back({"misra-c2012-rule-11-6", [] {
                 return castRule([](const Conversion &X, ASTContext &C) -> std::optional<std::string> {
                   if (!X.Explicit) return std::nullopt;
                   if (X.FromKind == Ptr::Void && X.To->isArithmeticType()) return "pointer to void cast to an arithmetic type";
                   if (X.ToKind == Ptr::Void && X.From->isArithmeticType()) {
                     auto V = integerConstant(X.E->getSubExpr(), C);
                     if (!(V && *V == 0)) return "arithmetic value cast to a pointer to void";
                   }
                   return std::nullopt;
                 });
               }});
  T.push_back({"misra-c2012-rule-11-7", [] {
                 return castRule([](const Conversion &X, ASTContext &C) -> std::optional<std::string> {
                   if (!X.Explicit) return std::nullopt;
                   auto NonInteger = [](Cat K) { return in(K, {Cat::Boolean, Cat::Character, Cat::Enum, Cat::Floating}); };
                   if (X.FromKind == Ptr::Object && NonInteger(essentialOfType(X.To, C).C))
                     return "pointer to object cast to a non-integer arithmetic type";
                   if (X.ToKind == Ptr::Object && NonInteger(essentialType(X.E->getSubExpr(), C).C))
                     return "non-integer arithmetic value cast to a pointer to object";
                   return std::nullopt;
                 });
               }});
  T.push_back({"misra-c2012-rule-11-8", [] {
                 return castRule([](const Conversion &X, ASTContext &) -> std::optional<std::string> {
                   if (!X.Explicit) return std::nullopt;
                   QualType A = X.From, B = X.To;
                   while (A->isPointerType() && B->isPointerType()) {
                     A = A->getPointeeType();
                     B = B->getPointeeType();
                     if ((A.isConstQualified() && !B.isConstQualified()) || (A.isVolatileQualified() && !B.isVolatileQualified()))
                       return "cast removes const or volatile qualification from the pointed-to type";
                   }
                   return std::nullopt;
                 });
               }});
  T.push_back({"misra-c2012-rule-11-9", [] {
                 return matcherSpec(implicitCastExpr(hasCastKind(CK_NullToPointer)).bind("c"), [](Rule &R, const Result &Res) {
                   const auto *E = Res.Nodes.getNodeAs<ImplicitCastExpr>("c");
                   const Expr *Sub = E->getSubExpr()->IgnoreParens();
                   const SourceManager &SM = *Res.SourceManager;
                   if (!Sub->getType()->isIntegerType() || !isUserLocation(SM, Sub->getExprLoc())) return;
                   if (!expandedFromMacro(SM, Res.Context->getLangOpts(), Sub->getBeginLoc(), "NULL"))
                     R.report(Sub->getBeginLoc(), "integer null pointer constant used instead of NULL");
                 });
               }});
}
} // namespace safety::misra
