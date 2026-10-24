// SPDX-License-Identifier: Apache-2.0
// Rules 12.x (expressions) and 13.x (side effects).
#include "Misra.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include <map>

using namespace clang;
using namespace clang::ast_matchers;

namespace safety::misra {
namespace {
// ---- Rule 12.1 ----------------------------------------------------------------
int precedence(const Expr *E) {
  E = E->IgnoreImplicit();
  if (isa<ParenExpr>(E)) return 16;
  if (isa<CallExpr, ArraySubscriptExpr, MemberExpr, CompoundLiteralExpr>(E)) return 15;
  if (const auto *U = dyn_cast<UnaryOperator>(E)) return U->isPostfix() ? 15 : 14;
  if (isa<UnaryExprOrTypeTraitExpr>(E)) return 14;
  if (isa<CStyleCastExpr>(E)) return 13;
  if (isa<ConditionalOperator>(E)) return 2;
  if (const auto *B = dyn_cast<BinaryOperator>(E)) {
    switch (B->getOpcode()) {
    case BO_Mul: case BO_Div: case BO_Rem: return 12;
    case BO_Add: case BO_Sub: return 11;
    case BO_Shl: case BO_Shr: return 10;
    case BO_LT: case BO_GT: case BO_LE: case BO_GE: return 9;
    case BO_EQ: case BO_NE: return 8;
    case BO_And: return 7;
    case BO_Xor: return 6;
    case BO_Or: return 5;
    case BO_LAnd: return 4;
    case BO_LOr: return 3;
    case BO_Comma: return 0;
    default: return 1;
    }
  }
  return 16;
}
class PrecedenceVisitor : public RecursiveASTVisitor<PrecedenceVisitor> {
public:
  PrecedenceVisitor(Rule &R, ASTContext &C) : R(R), C(C) {}
  void operands(const Expr *E, std::initializer_list<const Expr *> Ops) {
    int P = precedence(E);
    if (P < 2 || P > 12 || !isUserLocation(C.getSourceManager(), E->getExprLoc())) return;
    for (const Expr *Op : Ops) {
      int Q = precedence(Op);
      if (Q < 13 && Q > P) {
        R.report(Op->getBeginLoc(), "operand precedence should be made explicit with parentheses");
      }
    }
  }
  bool VisitBinaryOperator(BinaryOperator *B) { return operands(B, {B->getLHS(), B->getRHS()}), true; }
  bool VisitConditionalOperator(ConditionalOperator *E) {
    return operands(E, {E->getCond(), E->getTrueExpr(), E->getFalseExpr()}), true;
  }
  bool VisitUnaryExprOrTypeTraitExpr(UnaryExprOrTypeTraitExpr *E) {
    if (E->getKind() == UETT_SizeOf && !E->isArgumentType() && !isa<ParenExpr>(E->getArgumentExpr()) &&
        isUserLocation(C.getSourceManager(), E->getBeginLoc()))
      R.report(E->getBeginLoc(), "operand of sizeof should be enclosed in parentheses");
    return true;
  }

private:
  Rule &R;
  ASTContext &C;
};

// ---- Rule 12.4 ----------------------------------------------------------------
bool notEvaluated(ASTContext &C, const Expr *E) {
  const Stmt *Child = E;
  for (const Stmt *P = parentStmt(C, E); P; Child = P, P = parentStmt(C, P)) {
    if (const auto *Cond = dyn_cast<ConditionalOperator>(P)) {
      bool Value = false;
      if (Child != Cond->getCond() && !Cond->getCond()->containsErrors() &&
          Cond->getCond()->EvaluateAsBooleanCondition(Value, C) &&
          !Cond->getCond()->HasSideEffects(C))
        if ((Value && Child == Cond->getFalseExpr()) || (!Value && Child == Cond->getTrueExpr())) return true;
    }
    if (const auto *B = dyn_cast<BinaryOperator>(P); B && B->isLogicalOp() && Child == B->getRHS()) {
      bool Value = false;
      if (!B->getLHS()->containsErrors() && B->getLHS()->EvaluateAsBooleanCondition(Value, C) && Value == (B->getOpcode() == BO_LOr)) return true;
    }
  }
  return false;
}
std::optional<llvm::APSInt> exactValue(const Expr *E, ASTContext &C) {
  auto V = integerConstant(E, C);
  if (!V) return std::nullopt;
  llvm::APSInt X = V->extend(130);
  X.setIsSigned(true);
  return X;
}

// ---- Rule 13.2 ----------------------------------------------------------------
struct Access {
  std::string Key;
  bool Write;
  bool Volatile;
  std::vector<const Stmt *> Path;
};
class AccessCollector {
public:
  explicit AccessCollector(ASTContext &C) : C(C) {}
  void walk(const Stmt *S) {
    if (!S) return;
    if (const auto *T = dyn_cast<UnaryExprOrTypeTraitExpr>(S)) return; // unevaluated
    Path.push_back(S);
    if (const auto *B = dyn_cast<BinaryOperator>(S); B && B->isAssignmentOp()) {
      add(B->getLHS(), true, B->getLHS()->getType().isVolatileQualified());
    }
    if (const auto *U = dyn_cast<UnaryOperator>(S); U && U->isIncrementDecrementOp())
      add(U->getSubExpr(), true, U->getSubExpr()->getType().isVolatileQualified());
    if (const auto *Cast = dyn_cast<ImplicitCastExpr>(S); Cast && Cast->getCastKind() == CK_LValueToRValue)
      add(Cast->getSubExpr(), false, Cast->getSubExpr()->getType().isVolatileQualified());
    for (const Stmt *Child : S->children()) walk(Child);
    Path.pop_back();
  }
  void add(const Expr *E, bool Write, bool Volatile) {
    std::string Key = objectKey(E);
    if (Key.empty() && !Volatile) return;
    Accesses.push_back({Key, Write, Volatile, Path});
  }
  ASTContext &C;
  std::vector<const Stmt *> Path;
  std::vector<Access> Accesses;
};
// The deepest node shared by both paths, and the children taken below it.
const Stmt *commonAncestor(const Access &A, const Access &B, const Stmt *&ChildA, const Stmt *&ChildB) {
  size_t I = 0;
  while (I < A.Path.size() && I < B.Path.size() && A.Path[I] == B.Path[I]) ++I;
  if (I == 0) return nullptr;
  ChildA = I < A.Path.size() ? A.Path[I] : nullptr;
  ChildB = I < B.Path.size() ? B.Path[I] : nullptr;
  return A.Path[I - 1];
}
bool sequenced(const Stmt *Parent, const Stmt *ChildA, const Stmt *ChildB) {
  if (const auto *B = dyn_cast<BinaryOperator>(Parent))
    return B->isLogicalOp() || B->getOpcode() == BO_Comma;
  if (const auto *Cond = dyn_cast<ConditionalOperator>(Parent))
    return ChildA == Cond->getCond() || ChildB == Cond->getCond() || ChildA != ChildB;
  return false;
}
// A read that feeds the value stored by the write it shares an assignment with.
bool contributes(const Stmt *Parent, const Access &Write, const Stmt *ReadChild) {
  const auto *B = dyn_cast<BinaryOperator>(Parent);
  return B && B->isAssignmentOp() && ReadChild == B->getRHS() && Write.Path.back() == B;
}
std::optional<std::string> unsequencedConflict(const std::vector<Access> &All) {
  for (size_t I = 0; I < All.size(); ++I)
    for (size_t J = I + 1; J < All.size(); ++J) {
      const Access &A = All[I], &B = All[J];
      const Stmt *CA = nullptr, *CB = nullptr;
      const Stmt *Parent = commonAncestor(A, B, CA, CB);
      if (!Parent || sequenced(Parent, CA, CB)) continue;
      if (A.Volatile && B.Volatile && A.Write == B.Write)
        return std::string(A.Write ? "more than one volatile write" : "more than one volatile read") +
               " between sequence points";
      if (A.Key.empty() || A.Key != B.Key || (!A.Write && !B.Write)) continue;
      if (A.Write && B.Write) {
        if (A.Path.back() == B.Path.back()) continue;
        return "object is modified more than once between sequence points";
      }
      const Access &W = A.Write ? A : B;
      const Stmt *ReadChild = A.Write ? CB : CA;
      if (contributes(Parent, W, ReadChild)) continue;
      // The write's own operand, as in x++ or x += 1, is part of the same access.
      if (CA == nullptr || CB == nullptr) continue;
      return "object is both modified and read between sequence points";
    }
  return std::nullopt;
}
bool fullExpression(ASTContext &C, const Expr *E) {
  auto Parents = C.getParents(*E);
  if (Parents.empty()) return false;
  const auto *P = Parents[0].get<Stmt>();
  return !P || !isa<Expr>(P);
}
} // namespace

void addRulesExpressions(Table &T) {
  T.push_back({"misra-c2012-rule-12-1", [] {
                 return unitSpec([](Rule &R, ASTContext &C) { PrecedenceVisitor(R, C).TraverseDecl(C.getTranslationUnitDecl()); });
               }});
  T.push_back({"misra-c2012-rule-12-2", [] {
                 return matcherSpec(binaryOperator(hasAnyOperatorName("<<", ">>", "<<=", ">>=")).bind("b"), [](Rule &R, const Result &Res) {
                   const auto *B = Res.Nodes.getNodeAs<BinaryOperator>("b");
                   ASTContext &C = *Res.Context;
                   if (!isUserLocation(C.getSourceManager(), B->getOperatorLoc())) return;
                   auto V = integerConstant(B->getRHS(), C);
                   EType L = essentialType(B->getLHS(), C);
                   if (!V || !L.arithmetic()) return;
                   if (V->isNegative() || V->getExtValue() >= static_cast<int64_t>(L.Bits))
                     R.report(B->getOperatorLoc(), "shift amount is outside the width of the left operand's essential type");
                 });
               }});
  T.push_back({"misra-c2012-rule-12-3", [] {
                 return matcherSpec(binaryOperator(hasOperatorName(",")).bind("b"), [](Rule &R, const Result &Res) {
                   const auto *B = Res.Nodes.getNodeAs<BinaryOperator>("b");
                   if (isUserLocation(*Res.SourceManager, B->getOperatorLoc()))
                     R.report(B->getOperatorLoc(), "comma operator used");
                 });
               }});
  T.push_back({"misra-c2012-rule-12-4", [] {
                 Spec S;
                 S.Matchers = [](MatchFinder &F, Rule &R) {
                   F.addMatcher(binaryOperator(hasAnyOperatorName("+", "-", "*", "<<"), hasType(isUnsignedInteger())).bind("e"), &R);
                   F.addMatcher(unaryOperator(hasOperatorName("-"), hasType(isUnsignedInteger())).bind("e"), &R);
                 };
                 S.Check = [](Rule &R, const Result &Res) {
                   const auto *E = Res.Nodes.getNodeAs<Expr>("e");
                   ASTContext &C = *Res.Context;
                   if (!isUserLocation(C.getSourceManager(), E->getExprLoc()) || !integerConstant(E, C) || notEvaluated(C, E))
                     return;
                   unsigned Width = C.getIntWidth(E->getType());
                   llvm::APSInt Exact;
                   if (const auto *U = dyn_cast<UnaryOperator>(E)) {
                     auto V = exactValue(U->getSubExpr(), C);
                     if (!V) return;
                     Exact = -*V;
                   } else {
                     const auto *B = cast<BinaryOperator>(E);
                     auto L = exactValue(B->getLHS(), C), Rv = exactValue(B->getRHS(), C);
                     if (!L || !Rv) return;
                     switch (B->getOpcode()) {
                     case BO_Add: Exact = *L + *Rv; break;
                     case BO_Sub: Exact = *L - *Rv; break;
                     case BO_Mul: Exact = *L * *Rv; break;
                     default:
                       if (Rv->getExtValue() >= Width || Rv->isNegative()) return;
                       Exact = *L << static_cast<unsigned>(Rv->getExtValue());
                       break;
                     }
                   }
                   if (Exact.isNegative() || Exact.getActiveBits() > Width)
                     R.report(E->getExprLoc(), "constant expression wraps around in unsigned arithmetic");
                 };
                 return S;
               }});
  T.push_back({"misra-c2012-rule-13-1", [] {
                 return matcherSpec(initListExpr().bind("l"), [](Rule &R, const Result &Res) {
                   const auto *L = Res.Nodes.getNodeAs<InitListExpr>("l");
                   if (L->isSemanticForm() && L->getSyntacticForm()) L = L->getSyntacticForm();
                   for (const Expr *Init : L->inits()) {
                     if (!Init || isa<InitListExpr>(Init->IgnoreImplicit()) || !isUserLocation(*Res.SourceManager, Init->getBeginLoc()))
                       continue;
                     if (effectsOf(Init, false).total() > 0)
                       R.report(Init->getBeginLoc(), "initializer list element has persistent side effects");
                   }
                 });
               }});
  T.push_back({"misra-c2012-rule-13-2", [] {
                 return matcherSpec(expr().bind("e"), [](Rule &R, const Result &Res) {
                   const auto *E = Res.Nodes.getNodeAs<Expr>("e");
                   ASTContext &C = *Res.Context;
                   if (!isUserLocation(C.getSourceManager(), E->getBeginLoc()) || !fullExpression(C, E)) return;
                   AccessCollector A(C);
                   A.walk(E);
                   if (auto Why = unsequencedConflict(A.Accesses)) R.report(E->getBeginLoc(), *Why);
                 });
               }});
  T.push_back({"misra-c2012-rule-13-3", [] {
                 return matcherSpec(expr().bind("e"), [](Rule &R, const Result &Res) {
                   const auto *E = Res.Nodes.getNodeAs<Expr>("e");
                   ASTContext &C = *Res.Context;
                   if (!isUserLocation(C.getSourceManager(), E->getBeginLoc()) || !fullExpression(C, E)) return;
                   Effects X = effectsOf(E, true);
                   if (X.IncDec > 0 && X.total() > 1)
                     R.report(E->getBeginLoc(), "full expression combines ++ or -- with other side effects");
                 });
               }});
  T.push_back({"misra-c2012-rule-13-4", [] {
                 return matcherSpec(binaryOperator(isAssignmentOperator()).bind("b"), [](Rule &R, const Result &Res) {
                   const auto *B = Res.Nodes.getNodeAs<BinaryOperator>("b");
                   if (isUserLocation(*Res.SourceManager, B->getOperatorLoc()) && valueUse(*Res.Context, B) == Use::Used)
                     R.report(B->getOperatorLoc(), "result of an assignment operator is used");
                 });
               }});
  T.push_back({"misra-c2012-rule-13-5", [] {
                 return matcherSpec(binaryOperator(hasAnyOperatorName("&&", "||")).bind("b"), [](Rule &R, const Result &Res) {
                   const auto *B = Res.Nodes.getNodeAs<BinaryOperator>("b");
                   if (isUserLocation(*Res.SourceManager, B->getOperatorLoc()) && effectsOf(B->getRHS(), false).total() > 0)
                     R.report(B->getRHS()->getBeginLoc(), "right operand of a logical operator has persistent side effects");
                 });
               }});
  T.push_back({"misra-c2012-rule-13-6", [] {
                 return matcherSpec(unaryExprOrTypeTraitExpr(ofKind(UETT_SizeOf)).bind("s"), [](Rule &R, const Result &Res) {
                   const auto *S = Res.Nodes.getNodeAs<UnaryExprOrTypeTraitExpr>("s");
                   if (!isUserLocation(*Res.SourceManager, S->getBeginLoc())) return;
                   if (!S->isArgumentType()) {
                     const Expr *Arg = S->getArgumentExpr()->IgnoreParens();
                     if (isa<DeclRefExpr, MemberExpr>(Arg) && Arg->getType().isVolatileQualified() &&
                         !Arg->getType()->isVariableArrayType())
                       return;
                   }
                   Effects X = effectsOf(S, true);
                   if (X.total() > 0) R.report(S->getBeginLoc(), "operand of sizeof contains potential side effects");
                 });
               }});
}
} // namespace safety::misra
