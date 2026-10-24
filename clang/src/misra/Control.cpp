// SPDX-License-Identifier: Apache-2.0
// Rules 14.x-17.x: control statements, control flow, switch statements and
// functions.
#include "Misra.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include <algorithm>
#include <map>
#include <set>

using namespace clang;
using namespace clang::ast_matchers;

namespace safety::misra {
namespace {
// ---- helpers ------------------------------------------------------------------
struct Write {
  std::string Key;
  const Expr *Target;
};
void writes(const Stmt *S, std::vector<Write> &Out) {
  if (!S) return;
  if (const auto *B = dyn_cast<BinaryOperator>(S); B && B->isAssignmentOp()) Out.push_back({objectKey(B->getLHS()), B->getLHS()});
  if (const auto *U = dyn_cast<UnaryOperator>(S); U && U->isIncrementDecrementOp())
    Out.push_back({objectKey(U->getSubExpr()), U->getSubExpr()});
  if (const auto *U = dyn_cast<UnaryOperator>(S); U && U->getOpcode() == UO_AddrOf)
    Out.push_back({objectKey(U->getSubExpr()), U->getSubExpr()});
  for (const Stmt *Child : S->children()) writes(Child, Out);
}
std::set<std::string> writtenKeys(const Stmt *S) {
  std::vector<Write> W;
  writes(S, W);
  std::set<std::string> Out;
  for (const Write &X : W)
    if (!X.Key.empty()) Out.insert(X.Key);
  return Out;
}
void referenced(const Stmt *S, std::vector<const DeclRefExpr *> &Out) {
  if (!S) return;
  if (const auto *D = dyn_cast<DeclRefExpr>(S); D && isa<VarDecl>(D->getDecl())) Out.push_back(D);
  for (const Stmt *Child : S->children()) referenced(Child, Out);
}

// ---- Rule 14.2 ----------------------------------------------------------------
void forLoop(Rule &R, ASTContext &C, const ForStmt *F) {
  if (!F->getInit() && !F->getCond() && !F->getInc()) return;
  SourceLocation Loc = F->getForLoc();
  std::vector<Write> Inc;
  writes(F->getInc(), Inc);
  Effects IncEffects = effectsOf(F->getInc(), false);
  std::set<std::string> Counters;
  for (const Write &W : Inc) Counters.insert(W.Key);
  if (Counters.size() != 1 || Counters.count("") || IncEffects.Calls > 0 || IncEffects.VolatileRead > 0) {
    R.report(Loc, "for loop third clause must modify exactly one loop counter and nothing else");
    return;
  }
  std::string Counter = *Counters.begin();
  if (const Stmt *Init = F->getInit()) {
    bool Ok = false;
    if (const auto *D = dyn_cast<DeclStmt>(Init); D && D->isSingleDecl())
      Ok = D->getSingleDecl()->getAsFunction() == nullptr &&
           Counter == cast<NamedDecl>(D->getSingleDecl())->getNameAsString() + "@" +
                          std::to_string(reinterpret_cast<uintptr_t>(D->getSingleDecl()));
    if (const auto *B = dyn_cast<BinaryOperator>(Init); B && B->getOpcode() == BO_Assign)
      Ok = objectKey(B->getLHS()) == Counter && effectsOf(B->getRHS(), false).total() == 0;
    if (!Ok) R.report(Init->getBeginLoc(), "for loop first clause must only initialize the loop counter");
  }
  std::set<std::string> BodyWrites = writtenKeys(F->getBody());
  if (const Expr *Cond = F->getCond()) {
    if (effectsOf(Cond, false).total() > 0) R.report(Cond->getBeginLoc(), "for loop condition has persistent side effects");
    std::vector<const DeclRefExpr *> Refs;
    referenced(Cond, Refs);
    bool UsesCounter = false;
    for (const DeclRefExpr *D : Refs) {
      std::string Key = objectKey(D);
      UsesCounter |= llvm::StringRef(Counter).starts_with(Key) || Key == Counter;
      bool Flag = essentialOfType(D->getType(), C).C == Cat::Boolean;
      if (Key != Counter && BodyWrites.count(Key) && !Flag)
        R.report(D->getLocation(), "for loop condition uses an object modified in the loop body");
    }
    if (!UsesCounter) R.report(Cond->getBeginLoc(), "for loop condition does not use the loop counter");
  }
  std::vector<const DeclRefExpr *> IncRefs;
  referenced(F->getInc(), IncRefs);
  for (const DeclRefExpr *D : IncRefs)
    if (objectKey(D) != Counter && BodyWrites.count(objectKey(D)))
      R.report(D->getLocation(), "for loop third clause uses an object modified in the loop body");
  std::vector<Write> Body;
  writes(F->getBody(), Body);
  for (const Write &W : Body)
    if (W.Key == Counter) R.report(W.Target->getBeginLoc(), "loop counter is modified in the loop body");
}

// ---- Rule 14.3 ----------------------------------------------------------------
// Comparison of a non-constant operand with a constant outside its type range.
std::optional<bool> rangeInvariant(const Expr *E, ASTContext &C) {
  const auto *B = dyn_cast<BinaryOperator>(stripAll(E));
  if (!B || !B->isComparisonOp()) return std::nullopt;
  const Expr *L = stripAll(B->getLHS()), *R = stripAll(B->getRHS());
  BinaryOperatorKind Op = B->getOpcode();
  auto VR = integerConstant(R, C), VL = integerConstant(L, C);
  if (VL && !VR) {
    std::swap(L, R);
    std::swap(VL, VR);
    Op = BinaryOperator::reverseComparisonOp(Op);
  }
  if (!VR || VL || !L->getType()->isIntegerType()) return std::nullopt;
  unsigned W = C.getIntWidth(L->getType());
  bool Signed = L->getType()->isSignedIntegerType();
  llvm::APSInt Min = Signed ? llvm::APSInt::getMinValue(W, false) : llvm::APSInt::getMinValue(W, true);
  llvm::APSInt Max = Signed ? llvm::APSInt::getMaxValue(W, false) : llvm::APSInt::getMaxValue(W, true);
  auto Wide = [](llvm::APSInt X) {
    X = X.extend(130);
    X.setIsSigned(true);
    return X;
  };
  llvm::APSInt K = Wide(*VR), Lo = Wide(Min), Hi = Wide(Max);
  auto Holds = [&](const llvm::APSInt &X) {
    switch (Op) {
    case BO_LT: return X < K;
    case BO_GT: return X > K;
    case BO_LE: return X <= K;
    case BO_GE: return X >= K;
    case BO_EQ: return X == K;
    default: return X != K;
    }
  };
  if (Op == BO_EQ || Op == BO_NE) {
    if (K < Lo || K > Hi) return Op == BO_NE;
    return std::nullopt;
  }
  bool AtLo = Holds(Lo), AtHi = Holds(Hi);
  if (AtLo == AtHi) return AtLo;
  return std::nullopt;
}
std::optional<bool> invariant(const Expr *E, ASTContext &C) {
  if (!E || E->containsErrors() || E->HasSideEffects(C)) return std::nullopt;
  bool Value = false;
  if (E->EvaluateAsBooleanCondition(Value, C)) return Value;
  return rangeInvariant(E, C);
}

// ---- switch shape (Rules 16.x) ----------------------------------------------------
struct Clause {
  std::vector<const SwitchCase *> Labels;
  std::vector<const Stmt *> Body;
};
struct Shape {
  bool Compound = false, StartsWithLabel = true;
  std::vector<Clause> Clauses;
  std::vector<const SwitchCase *> Nested;
  const DefaultStmt *Default = nullptr;
  std::vector<const SwitchCase *> Ordered;
};
Shape shapeOf(const SwitchStmt *S, const SourceManager &SM) {
  Shape X;
  std::set<const SwitchCase *> Direct;
  if (const auto *Body = dyn_cast_or_null<CompoundStmt>(S->getBody())) {
    X.Compound = true;
    for (const Stmt *Child : Body->body()) {
      if (isa<SwitchCase>(Child)) {
        X.Clauses.emplace_back();
        const Stmt *Cur = Child;
        while (const auto *Label = dyn_cast<SwitchCase>(Cur)) {
          X.Clauses.back().Labels.push_back(Label);
          Direct.insert(Label);
          Cur = Label->getSubStmt();
        }
        X.Clauses.back().Body.push_back(Cur);
      } else if (X.Clauses.empty())
        X.StartsWithLabel = false;
      else
        X.Clauses.back().Body.push_back(Child);
    }
  }
  for (const SwitchCase *Case = S->getSwitchCaseList(); Case; Case = Case->getNextSwitchCase()) {
    X.Ordered.push_back(Case);
    if (const auto *D = dyn_cast<DefaultStmt>(Case)) X.Default = D;
    if (!Direct.count(Case)) X.Nested.push_back(Case);
  }
  std::sort(X.Ordered.begin(), X.Ordered.end(), [&](const SwitchCase *A, const SwitchCase *B) {
    return SM.isBeforeInTranslationUnit(A->getBeginLoc(), B->getBeginLoc());
  });
  return X;
}
bool endsWithBreak(const Clause &Cl) {
  if (Cl.Body.empty()) return false;
  const Stmt *Last = Cl.Body.back();
  if (const auto *Block = dyn_cast<CompoundStmt>(Last)) Last = Block->body_empty() ? nullptr : Block->body_back();
  return isa_and_nonnull<BreakStmt>(Last);
}
bool defaultMisplaced(const Shape &X) {
  return X.Default && X.Ordered.size() > 2 && X.Ordered.front() != X.Default && X.Ordered.back() != X.Default;
}

// ---- Rule 15.3 ----------------------------------------------------------------
// Position of a statement as (enclosing compound, switch clause index or -1).
std::pair<const CompoundStmt *, int> blockOf(ASTContext &C, const Stmt *S, const Stmt *Stop = nullptr) {
  const Stmt *Child = S;
  for (const Stmt *P = parentStmt(C, S); P; Child = P, P = parentStmt(C, P)) {
    if (P == Stop) break;
    if (isa<LabelStmt, SwitchCase>(P) && cast<Stmt>(P)->child_begin() != cast<Stmt>(P)->child_end() &&
        isa<LabelStmt>(P))
      continue;
    const auto *Block = dyn_cast<CompoundStmt>(P);
    if (!Block) continue;
    int Clause = -1;
    if (isa_and_nonnull<SwitchStmt>(parentStmt(C, Block))) {
      Clause = 0;
      for (const Stmt *Item : Block->body()) {
        if (isa<SwitchCase>(Item)) ++Clause;
        if (Item == Child) break;
      }
    }
    return {Block, Clause};
  }
  return {nullptr, -1};
}
bool gotoReachesLabel(ASTContext &C, const GotoStmt *G, const LabelStmt *L) {
  auto [LabelBlock, LabelClause] = blockOf(C, L);
  const Stmt *Child = G;
  for (const Stmt *P = parentStmt(C, G); P; Child = P, P = parentStmt(C, P)) {
    if (P != LabelBlock) continue;
    if (LabelClause < 0) return true;
    int Clause = 0;
    for (const Stmt *Item : LabelBlock->body()) {
      if (isa<SwitchCase>(Item)) ++Clause;
      if (Item == Child) break;
    }
    return Clause == LabelClause;
  }
  return false;
}

// ---- Rule 15.4 ----------------------------------------------------------------
void loopExits(const Stmt *S, const Stmt *Loop, SourceManager &SM, unsigned &Count, bool BreaksCount) {
  if (!S) return;
  if (isa<BreakStmt>(S) && BreaksCount) ++Count;
  if (const auto *G = dyn_cast<GotoStmt>(S)) {
    SourceLocation L = G->getLabel()->getLocation();
    if (!SM.isPointWithin(L, Loop->getBeginLoc(), Loop->getEndLoc())) ++Count;
  }
  bool Nested = isa<WhileStmt, DoStmt, ForStmt, SwitchStmt>(S) && S != Loop;
  for (const Stmt *Child : S->children()) loopExits(Child, Loop, SM, Count, BreaksCount && !Nested);
}

// ---- Rule 17.2 ----------------------------------------------------------------
class Calls : public RecursiveASTVisitor<Calls> {
public:
  bool TraverseFunctionDecl(FunctionDecl *F) {
    const FunctionDecl *Saved = Current;
    if (F->doesThisDeclarationHaveABody()) Current = F->getCanonicalDecl();
    bool Continue = RecursiveASTVisitor::TraverseFunctionDecl(F);
    Current = Saved;
    return Continue;
  }
  bool VisitCallExpr(CallExpr *E) {
    if (Current)
      if (const auto *F = E->getDirectCallee()) Graph[Current].insert(F->getCanonicalDecl());
    return true;
  }
  const FunctionDecl *Current = nullptr;
  std::map<const FunctionDecl *, std::set<const FunctionDecl *>> Graph;
};
bool reaches(const std::map<const FunctionDecl *, std::set<const FunctionDecl *>> &G, const FunctionDecl *From,
             const FunctionDecl *Target) {
  std::set<const FunctionDecl *> Seen;
  std::vector<const FunctionDecl *> Work{From};
  while (!Work.empty()) {
    const FunctionDecl *F = Work.back();
    Work.pop_back();
    auto It = G.find(F);
    if (It == G.end()) continue;
    for (const FunctionDecl *Callee : It->second) {
      if (Callee == Target) return true;
      if (Seen.insert(Callee).second) Work.push_back(Callee);
    }
  }
  return false;
}

bool vaListType(QualType T, ASTContext &C) {
  while (!T.isNull()) {
    if (const auto *TT = T->getAs<TypedefType>()) {
      llvm::StringRef Name = TT->getDecl()->getName();
      if (Name == "va_list" || Name == "__gnuc_va_list" || Name == "__builtin_va_list") return true;
      T = TT->getDecl()->getUnderlyingType();
      continue;
    }
    break;
  }
  return !T.isNull() && C.hasSameType(T, C.getBuiltinVaListType());
}
} // namespace

void addRulesControl(Table &T) {
  T.push_back({"misra-c2012-rule-14-1", [] {
                 Spec S;
                 S.Matchers = [](MatchFinder &F, Rule &R) {
                   F.addMatcher(forStmt().bind("loop"), &R);
                   F.addMatcher(whileStmt().bind("loop"), &R);
                   F.addMatcher(doStmt().bind("loop"), &R);
                 };
                 S.Check = [](Rule &R, const Result &Res) {
                   const auto *L = Res.Nodes.getNodeAs<Stmt>("loop");
                   ASTContext &C = *Res.Context;
                   if (!isUserLocation(C.getSourceManager(), L->getBeginLoc())) return;
                   const Expr *Cond = nullptr;
                   const Stmt *Changes = nullptr;
                   if (const auto *F = dyn_cast<ForStmt>(L)) Cond = F->getCond(), Changes = F;
                   if (const auto *W = dyn_cast<WhileStmt>(L)) Cond = W->getCond(), Changes = W->getBody();
                   if (const auto *D = dyn_cast<DoStmt>(L)) Cond = D->getCond(), Changes = D->getBody();
                   std::set<std::string> Modified = writtenKeys(Changes);
                   std::vector<const DeclRefExpr *> Refs;
                   referenced(Cond, Refs);
                   for (const DeclRefExpr *D : Refs)
                     if (D->getType()->isRealFloatingType() && Modified.count(objectKey(D)))
                       R.report(D->getLocation(), "loop counter has essentially floating type");
                 };
                 return S;
               }});
  T.push_back({"misra-c2012-rule-14-2", [] {
                 return matcherSpec(forStmt().bind("f"), [](Rule &R, const Result &Res) {
                   const auto *F = Res.Nodes.getNodeAs<ForStmt>("f");
                   if (isUserLocation(*Res.SourceManager, F->getForLoc())) forLoop(R, *Res.Context, F);
                 });
               }});
  T.push_back({"misra-c2012-rule-14-3", [] {
                 Spec S;
                 S.Matchers = [](MatchFinder &F, Rule &R) {
                   F.addMatcher(stmt(anyOf(ifStmt(), whileStmt(), doStmt(), forStmt(), switchStmt(), conditionalOperator())).bind("s"), &R);
                 };
                 S.Check = [](Rule &R, const Result &Res) {
                   const auto *St = Res.Nodes.getNodeAs<Stmt>("s");
                   ASTContext &C = *Res.Context;
                   if (!isUserLocation(C.getSourceManager(), St->getBeginLoc())) return;
                   const Expr *Cond = nullptr;
                   bool Loop = false, Do = false;
                   if (const auto *I = dyn_cast<IfStmt>(St)) Cond = I->getCond();
                   if (const auto *W = dyn_cast<WhileStmt>(St)) Cond = W->getCond(), Loop = true;
                   if (const auto *D = dyn_cast<DoStmt>(St)) Cond = D->getCond(), Loop = Do = true;
                   if (const auto *F = dyn_cast<ForStmt>(St)) Cond = F->getCond(), Loop = true;
                   if (const auto *Sw = dyn_cast<SwitchStmt>(St)) Cond = Sw->getCond();
                   if (const auto *Co = dyn_cast<ConditionalOperator>(St)) Cond = Co->getCond();
                   if (!Cond) return;
                   if (isa<SwitchStmt>(St)) {
                     if (integerConstant(Cond, C)) R.report(Cond->getBeginLoc(), "switch controlling expression is invariant");
                     return;
                   }
                   auto V = invariant(Cond, C);
                   if (!V) return;
                   if (Loop && *V && isConstantExpression(Cond, C)) return; // deliberate infinite loop
                   if (Do && !*V && essentialType(Cond, C).C == Cat::Boolean) return;
                   R.report(Cond->getBeginLoc(), "controlling expression is invariant");
                 };
                 return S;
               }});
  T.push_back({"misra-c2012-rule-14-4", [] {
                 Spec S;
                 S.Matchers = [](MatchFinder &F, Rule &R) {
                   F.addMatcher(stmt(anyOf(ifStmt(), whileStmt(), doStmt(), forStmt())).bind("s"), &R);
                 };
                 S.Check = [](Rule &R, const Result &Res) {
                   const auto *St = Res.Nodes.getNodeAs<Stmt>("s");
                   const Expr *Cond = nullptr;
                   if (const auto *I = dyn_cast<IfStmt>(St)) Cond = I->getCond();
                   if (const auto *W = dyn_cast<WhileStmt>(St)) Cond = W->getCond();
                   if (const auto *D = dyn_cast<DoStmt>(St)) Cond = D->getCond();
                   if (const auto *F = dyn_cast<ForStmt>(St)) Cond = F->getCond();
                   if (Cond && isUserLocation(*Res.SourceManager, Cond->getBeginLoc()) &&
                       essentialType(Cond, *Res.Context).C != Cat::Boolean)
                     R.report(Cond->getBeginLoc(), "controlling expression is not essentially Boolean");
                 };
                 return S;
               }});
  T.push_back({"misra-c2012-rule-15-1", [] {
                 return matcherSpec(gotoStmt().bind("g"), [](Rule &R, const Result &Res) {
                   const auto *G = Res.Nodes.getNodeAs<GotoStmt>("g");
                   if (isUserLocation(*Res.SourceManager, G->getGotoLoc())) R.report(G->getGotoLoc(), "goto statement used");
                 });
               }});
  T.push_back({"misra-c2012-rule-15-2", [] {
                 return matcherSpec(gotoStmt().bind("g"), [](Rule &R, const Result &Res) {
                   const auto *G = Res.Nodes.getNodeAs<GotoStmt>("g");
                   const SourceManager &SM = *Res.SourceManager;
                   const LabelStmt *L = G->getLabel()->getStmt();
                   if (isUserLocation(SM, G->getGotoLoc()) && L && SM.isBeforeInTranslationUnit(L->getBeginLoc(), G->getGotoLoc()))
                     R.report(G->getGotoLoc(), "goto jumps backwards to an earlier label");
                 });
               }});
  T.push_back({"misra-c2012-rule-15-3", [] {
                 return matcherSpec(gotoStmt().bind("g"), [](Rule &R, const Result &Res) {
                   const auto *G = Res.Nodes.getNodeAs<GotoStmt>("g");
                   const LabelStmt *L = G->getLabel()->getStmt();
                   if (isUserLocation(*Res.SourceManager, G->getGotoLoc()) && L && !gotoReachesLabel(*Res.Context, G, L))
                     R.report(G->getGotoLoc(), "goto target label is not in the same or an enclosing block");
                 });
               }});
  T.push_back({"misra-c2012-rule-15-4", [] {
                 return matcherSpec(stmt(anyOf(forStmt(), whileStmt(), doStmt())).bind("l"), [](Rule &R, const Result &Res) {
                   const auto *L = Res.Nodes.getNodeAs<Stmt>("l");
                   if (!isUserLocation(*Res.SourceManager, L->getBeginLoc())) return;
                   unsigned Count = 0;
                   for (const Stmt *Child : L->children()) loopExits(Child, L, *Res.SourceManager, Count, true);
                   if (Count > 1) R.report(L->getBeginLoc(), "loop has more than one break or goto that terminates it");
                 });
               }});
  T.push_back({"misra-c2012-rule-15-5", [] {
                 return matcherSpec(functionDecl(isDefinition()).bind("f"), [](Rule &R, const Result &Res) {
                   const auto *F = Res.Nodes.getNodeAs<FunctionDecl>("f");
                   const auto *Body = dyn_cast_or_null<CompoundStmt>(F->getBody());
                   if (!isUserDecl(F) || !Body) return;
                   struct V : RecursiveASTVisitor<V> {
                     std::vector<const ReturnStmt *> Returns;
                     bool VisitReturnStmt(ReturnStmt *S) { return Returns.push_back(S), true; }
                   } Walk;
                   Walk.TraverseStmt(const_cast<CompoundStmt *>(Body));
                   const Stmt *Last = Body->body_empty() ? nullptr : Body->body_back();
                   for (const ReturnStmt *Ret : Walk.Returns)
                     if (Ret != Last) R.report(Ret->getReturnLoc(), "function has an exit point other than at its end");
                 });
               }});
  T.push_back({"misra-c2012-rule-15-6", [] {
                 Spec S;
                 S.Matchers = [](MatchFinder &F, Rule &R) {
                   F.addMatcher(stmt(anyOf(ifStmt(), whileStmt(), doStmt(), forStmt(), switchStmt())).bind("s"), &R);
                 };
                 S.Check = [](Rule &R, const Result &Res) {
                   const auto *St = Res.Nodes.getNodeAs<Stmt>("s");
                   if (!isUserLocation(*Res.SourceManager, St->getBeginLoc())) return;
                   std::vector<const Stmt *> Bodies;
                   if (const auto *I = dyn_cast<IfStmt>(St)) {
                     Bodies.push_back(I->getThen());
                     if (I->getElse() && !isa<IfStmt>(I->getElse())) Bodies.push_back(I->getElse());
                   }
                   if (const auto *W = dyn_cast<WhileStmt>(St)) Bodies.push_back(W->getBody());
                   if (const auto *D = dyn_cast<DoStmt>(St)) Bodies.push_back(D->getBody());
                   if (const auto *F = dyn_cast<ForStmt>(St)) Bodies.push_back(F->getBody());
                   if (const auto *Sw = dyn_cast<SwitchStmt>(St)) Bodies.push_back(Sw->getBody());
                   for (const Stmt *B : Bodies)
                     if (B && !isa<CompoundStmt>(B)) R.report(B->getBeginLoc(), "body of a control statement is not a compound statement");
                 };
                 return S;
               }});
  T.push_back({"misra-c2012-rule-15-7", [] {
                 return matcherSpec(ifStmt(hasElse(ifStmt())).bind("i"), [](Rule &R, const Result &Res) {
                   const auto *I = Res.Nodes.getNodeAs<IfStmt>("i");
                   ASTContext &C = *Res.Context;
                   const auto *Outer = dyn_cast_or_null<IfStmt>(parentStmt(C, I));
                   if (!isUserLocation(C.getSourceManager(), I->getIfLoc()) || (Outer && Outer->getElse() == I)) return;
                   const IfStmt *Last = I;
                   while (const auto *Next = dyn_cast_or_null<IfStmt>(Last->getElse())) Last = Next;
                   if (!Last->getElse()) {
                     R.report(Last->getIfLoc(), "if ... else if chain is not terminated by an else");
                     return;
                   }
                   const auto *Final = dyn_cast<CompoundStmt>(Last->getElse());
                   if (!Final) return;
                   bool Action = false;
                   for (const Stmt *S : Final->body()) Action |= !isa<NullStmt>(S);
                   if (!Action && !rangeHasComment(C.getSourceManager(), C.getLangOpts(),
                                                   SourceRange(Final->getLBracLoc().getLocWithOffset(1), Final->getRBracLoc())))
                     R.report(Last->getElseLoc(), "final else has neither a side effect nor an explanatory comment");
                 });
               }});
  auto SwitchRule = [](std::function<void(Rule &, ASTContext &, const SwitchStmt *, const Shape &)> F) {
    return [F] {
      return matcherSpec(switchStmt().bind("s"), [F](Rule &R, const Result &Res) {
        const auto *S = Res.Nodes.getNodeAs<SwitchStmt>("s");
        if (isUserLocation(*Res.SourceManager, S->getSwitchLoc())) F(R, *Res.Context, S, shapeOf(S, *Res.SourceManager));
      });
    };
  };
  T.push_back({"misra-c2012-rule-16-1", SwitchRule([](Rule &R, ASTContext &, const SwitchStmt *S, const Shape &X) {
                 bool Breaks = std::all_of(X.Clauses.begin(), X.Clauses.end(), endsWithBreak);
                 if (!X.Compound || !X.StartsWithLabel || !X.Nested.empty() || !Breaks || !X.Default || defaultMisplaced(X) ||
                     X.Clauses.size() < 2)
                   R.report(S->getSwitchLoc(), "switch statement is not well-formed");
               })});
  T.push_back({"misra-c2012-rule-16-2", SwitchRule([](Rule &R, ASTContext &, const SwitchStmt *, const Shape &X) {
                 for (const SwitchCase *Label : X.Nested)
                   R.report(Label->getKeywordLoc(), "switch label is not directly within the switch body");
               })});
  T.push_back({"misra-c2012-rule-16-3", SwitchRule([](Rule &R, ASTContext &, const SwitchStmt *, const Shape &X) {
                 for (const Clause &Cl : X.Clauses)
                   if (!endsWithBreak(Cl)) R.report(Cl.Labels.front()->getKeywordLoc(), "switch clause is not terminated by an unconditional break");
               })});
  T.push_back({"misra-c2012-rule-16-4", SwitchRule([](Rule &R, ASTContext &C, const SwitchStmt *S, const Shape &X) {
                 if (!X.Default) {
                   R.report(S->getSwitchLoc(), "switch statement has no default label");
                   return;
                 }
                 for (const Clause &Cl : X.Clauses) {
                   if (std::find(Cl.Labels.begin(), Cl.Labels.end(), X.Default) == Cl.Labels.end()) continue;
                   if (Cl.Body.size() == 1 && isa<BreakStmt>(Cl.Body.front()) &&
                       !rangeHasComment(C.getSourceManager(), C.getLangOpts(),
                                        SourceRange(X.Default->getColonLoc().getLocWithOffset(1), Cl.Body.front()->getBeginLoc())))
                     R.report(X.Default->getKeywordLoc(), "default clause has neither a statement nor a comment");
                 }
               })});
  T.push_back({"misra-c2012-rule-16-5", SwitchRule([](Rule &R, ASTContext &, const SwitchStmt *, const Shape &X) {
                 if (defaultMisplaced(X)) R.report(X.Default->getKeywordLoc(), "default label is neither the first nor the last switch label");
               })});
  T.push_back({"misra-c2012-rule-16-6", SwitchRule([](Rule &R, ASTContext &, const SwitchStmt *S, const Shape &X) {
                 if (X.Compound && X.Clauses.size() < 2) R.report(S->getSwitchLoc(), "switch statement has fewer than two clauses");
               })});
  T.push_back({"misra-c2012-rule-16-7", SwitchRule([](Rule &R, ASTContext &C, const SwitchStmt *S, const Shape &) {
                 if (essentialType(S->getCond(), C).C == Cat::Boolean)
                   R.report(S->getCond()->getBeginLoc(), "switch controlling expression is essentially Boolean");
               })});
  T.push_back({"misra-c2012-rule-17-1", [] {
                 Spec S;
                 S.Matchers = [](MatchFinder &F, Rule &R) {
                   F.addMatcher(expr().bind("arg"), &R);
                   F.addMatcher(declaratorDecl().bind("d"), &R);
                   F.addMatcher(callExpr(callee(functionDecl(hasAnyName("__builtin_va_start", "__builtin_va_end", "__builtin_va_copy",
                                                                          "__builtin_c23_va_start"))))
                                    .bind("call"),
                                &R);
                 };
                 S.Check = [](Rule &R, const Result &Res) {
                   SourceLocation L;
                   if (const auto *A = dyn_cast_or_null<VAArgExpr>(Res.Nodes.getNodeAs<Expr>("arg"))) L = A->getBeginLoc();
                   if (const auto *Call = Res.Nodes.getNodeAs<CallExpr>("call")) L = Call->getBeginLoc();
                   if (const auto *D = Res.Nodes.getNodeAs<DeclaratorDecl>("d")) {
                     QualType T = isa<ParmVarDecl>(D) ? cast<ParmVarDecl>(D)->getOriginalType() : D->getType();
                     if (!isa<FunctionDecl>(D) && vaListType(T, *Res.Context)) L = D->getLocation();
                   }
                   if (isUserLocation(*Res.SourceManager, L)) R.report(L, "<stdarg.h> feature used");
                 };
                 return S;
               }});
  T.push_back({"misra-c2012-rule-17-2", [] {
                 return unitSpec([](Rule &R, ASTContext &C) {
                   Calls V;
                   V.TraverseDecl(C.getTranslationUnitDecl());
                   for (const auto &[F, Callees] : V.Graph) {
                     (void)Callees;
                     const FunctionDecl *Def = F->getDefinition() ? F->getDefinition() : F;
                     if (isUserDecl(Def) && reaches(V.Graph, F, F))
                       R.report(Def->getLocation(), "function calls itself directly or indirectly");
                   }
                 });
               }});
  T.push_back({"misra-c2012-rule-17-3", [] {
                 return matcherSpec(callExpr(callee(functionDecl(isImplicit()))).bind("c"), [](Rule &R, const Result &Res) {
                   const auto *Call = Res.Nodes.getNodeAs<CallExpr>("c");
                   if (isUserLocation(*Res.SourceManager, Call->getBeginLoc()))
                     R.report(Call->getBeginLoc(), "function is called without a visible declaration");
                 });
               }});
  T.push_back({"misra-c2012-rule-17-4", [] {
                 Spec S;
                 S.Matchers = [](MatchFinder &F, Rule &R) {
                   F.addMatcher(functionDecl(isDefinition()).bind("f"), &R);
                   F.addMatcher(returnStmt(unless(has(expr())), forFunction(functionDecl(unless(returns(voidType()))))).bind("r"), &R);
                 };
                 S.Check = [](Rule &R, const Result &Res) {
                   if (const auto *Ret = Res.Nodes.getNodeAs<ReturnStmt>("r")) {
                     if (isUserLocation(*Res.SourceManager, Ret->getReturnLoc()))
                       R.report(Ret->getReturnLoc(), "return statement in a non-void function has no value");
                     return;
                   }
                   const auto *F = Res.Nodes.getNodeAs<FunctionDecl>("f");
                   if (!isUserDecl(F) || F->getReturnType()->isVoidType() || F->isNoReturn()) return;
                   const auto *Body = dyn_cast_or_null<CompoundStmt>(F->getBody());
                   if (Body && canCompleteNormally(Body, *Res.Context))
                     R.report(Body->getRBracLoc(), "control can reach the end of a non-void function without a return value");
                 };
                 return S;
               }});
  T.push_back({"misra-c2012-rule-17-5", [] {
                 return matcherSpec(callExpr(callee(functionDecl())).bind("c"), [](Rule &R, const Result &Res) {
                   const auto *Call = Res.Nodes.getNodeAs<CallExpr>("c");
                   const FunctionDecl *F = Call->getDirectCallee();
                   ASTContext &C = *Res.Context;
                   if (!isUserLocation(C.getSourceManager(), Call->getBeginLoc())) return;
                   for (unsigned I = 0; I < Call->getNumArgs() && I < F->getNumParams(); ++I) {
                     const auto *Param = C.getAsConstantArrayType(F->getParamDecl(I)->getOriginalType());
                     const auto *Arg = C.getAsConstantArrayType(stripAll(Call->getArg(I))->getType());
                     if (Param && Arg && Arg->getSize().ult(Param->getSize()))
                       R.report(Call->getArg(I)->getBeginLoc(), "array argument has fewer elements than the parameter declares");
                   }
                 });
               }});
  T.push_back({"misra-c2012-rule-17-6", [] {
                 return matcherSpec(parmVarDecl().bind("p"), [](Rule &R, const Result &Res) {
                   const auto *P = Res.Nodes.getNodeAs<ParmVarDecl>("p");
                   const auto *A = Res.Context->getAsArrayType(P->getOriginalType());
                   if (isUserDecl(P) && A && A->getSizeModifier() == ArraySizeModifier::Static)
                     R.report(P->getLocation(), "array parameter uses 'static' in its declarator");
                 });
               }});
  T.push_back({"misra-c2012-rule-17-7", [] {
                 return matcherSpec(callExpr().bind("c"), [](Rule &R, const Result &Res) {
                   const auto *Call = Res.Nodes.getNodeAs<CallExpr>("c");
                   if (Call->getCallReturnType(*Res.Context)->isVoidType() || !isUserLocation(*Res.SourceManager, Call->getBeginLoc()))
                     return;
                   if (valueUse(*Res.Context, Call) == Use::Discarded)
                     R.report(Call->getBeginLoc(), "value returned by a non-void function is discarded");
                 });
               }});
  T.push_back({"misra-c2012-rule-17-8", [] {
                 Spec S;
                 S.Matchers = [](MatchFinder &F, Rule &R) {
                   F.addMatcher(binaryOperator(isAssignmentOperator(), hasLHS(ignoringParens(declRefExpr(to(parmVarDecl()))))).bind("m"), &R);
                   F.addMatcher(unaryOperator(hasAnyOperatorName("++", "--"), hasUnaryOperand(ignoringParens(declRefExpr(to(parmVarDecl())))))
                                    .bind("m"),
                                &R);
                 };
                 S.Check = [](Rule &R, const Result &Res) {
                   const auto *E = Res.Nodes.getNodeAs<Expr>("m");
                   if (isUserLocation(*Res.SourceManager, E->getExprLoc())) R.report(E->getExprLoc(), "function parameter is modified");
                 };
                 return S;
               }});
}
} // namespace safety::misra
