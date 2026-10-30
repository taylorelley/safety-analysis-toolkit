// SPDX-License-Identifier: Apache-2.0
#include "Misra.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Lex/Lexer.h"
#include "llvm/ADT/StringSet.h"
#include <algorithm>

using namespace clang;

namespace safety::misra {
void Rule::report(SourceLocation Loc, llvm::StringRef Message) {
  // Error recovery can synthesise declarations without a location; such a
  // diagnostic cannot be attributed to source and would fail the whole unit.
  if (Loc.isInvalid()) return;
  // Diagnostics inside system macros are dropped by clang-tidy; use the
  // location where the macro was expanded instead.
  if (Sources && Loc.isMacroID() && Sources->isInSystemHeader(Sources->getSpellingLoc(Loc)))
    Loc = Sources->getExpansionLoc(Loc);
  if (std::find(Reported.begin(), Reported.end(), Loc) != Reported.end()) return;
  Reported.push_back(Loc);
  diag(Loc, Message);
}

Spec unitSpec(std::function<void(Rule &, ASTContext &)> Run) {
  return matcherSpec(am::translationUnitDecl().bind("unit"),
                     [Run](Rule &R, const Result &Res) { Run(R, *Res.Context); });
}

bool isUserLocation(const SourceManager &SM, SourceLocation L) {
  if (L.isInvalid()) return false;
  L = SM.getExpansionLoc(L);
  return !SM.isInSystemHeader(L) && SM.getFileEntryRefForID(SM.getFileID(L)).has_value();
}
bool isUserDecl(const Decl *D) {
  return D && !D->isImplicit() && isUserLocation(D->getASTContext().getSourceManager(), D->getLocation());
}

const Stmt *parentStmt(ASTContext &C, const Stmt *S) {
  auto Parents = C.getParents(*S);
  return Parents.empty() ? nullptr : Parents[0].get<Stmt>();
}
const Decl *parentDecl(ASTContext &C, const Stmt *S) {
  auto Parents = C.getParents(*S);
  return Parents.empty() ? nullptr : Parents[0].get<Decl>();
}
const Expr *stripParens(const Expr *E) { return E ? E->IgnoreParens() : nullptr; }
const Expr *stripAll(const Expr *E) { return E ? E->IgnoreParenImpCasts() : nullptr; }

Use valueUse(ASTContext &C, const Expr *E) {
  const Stmt *Child = E;
  while (const Stmt *P = parentStmt(C, Child)) {
    if (isa<ParenExpr>(P)) {
      Child = P;
      continue;
    }
    if (const auto *B = dyn_cast<BinaryOperator>(P); B && B->getOpcode() == BO_Comma) {
      if (B->getLHS() == Child) return Use::Discarded;
      Child = P;
      continue;
    }
    if (const auto *Cast = dyn_cast<CStyleCastExpr>(P); Cast && Cast->getType()->isVoidType())
      return Use::VoidCast;
    if (isa<CompoundStmt, LabelStmt, DefaultStmt>(P)) return Use::Discarded;
    if (const auto *Case = dyn_cast<CaseStmt>(P)) return Case->getSubStmt() == Child ? Use::Discarded : Use::Used;
    if (const auto *If = dyn_cast<IfStmt>(P)) return If->getCond() == Child ? Use::Used : Use::Discarded;
    if (const auto *W = dyn_cast<WhileStmt>(P)) return W->getCond() == Child ? Use::Used : Use::Discarded;
    if (const auto *D = dyn_cast<DoStmt>(P)) return D->getCond() == Child ? Use::Used : Use::Discarded;
    if (const auto *F = dyn_cast<ForStmt>(P)) return F->getCond() == Child ? Use::Used : Use::Discarded;
    if (const auto *Sw = dyn_cast<SwitchStmt>(P)) return Sw->getCond() == Child ? Use::Used : Use::Discarded;
    return Use::Used;
  }
  return Use::Used;
}

bool isVolatileRead(const Expr *E) {
  const auto *Cast = dyn_cast<ImplicitCastExpr>(E);
  return Cast && Cast->getCastKind() == CK_LValueToRValue && Cast->getSubExpr()->getType().isVolatileQualified();
}

static void collectEffects(const Stmt *S, bool Unevaluated, Effects &Out) {
  if (!S) return;
  if (const auto *U = dyn_cast<UnaryOperator>(S); U && U->isIncrementDecrementOp()) ++Out.IncDec;
  if (const auto *B = dyn_cast<BinaryOperator>(S); B && B->isAssignmentOp()) ++Out.Assign;
  if (const auto *Call = dyn_cast<CallExpr>(S)) {
    const auto *F = Call->getDirectCallee();
    if (!F || !(F->hasAttr<ConstAttr>() || F->hasAttr<PureAttr>())) ++Out.Calls;
  }
  if (const auto *E = dyn_cast<Expr>(S); E && isVolatileRead(E)) ++Out.VolatileRead;
  if (const auto *Trait = dyn_cast<UnaryExprOrTypeTraitExpr>(S)) {
    if (!Unevaluated) return;
    if (Trait->isArgumentType()) {
      // Variable-length array bounds inside a type operand can be evaluated.
      QualType T = Trait->getArgumentType();
      while (const auto *A = T->getAsArrayTypeUnsafe()) {
        if (const auto *V = dyn_cast<VariableArrayType>(A)) collectEffects(V->getSizeExpr(), Unevaluated, Out);
        T = A->getElementType();
      }
      return;
    }
  }
  for (const Stmt *Child : S->children()) collectEffects(Child, Unevaluated, Out);
}
Effects effectsOf(const Stmt *S, bool Unevaluated) {
  Effects E;
  collectEffects(S, Unevaluated, E);
  return E;
}

const VarDecl *namedVariable(const Expr *E) {
  E = stripAll(E);
  if (const auto *D = dyn_cast_or_null<DeclRefExpr>(E)) return dyn_cast<VarDecl>(D->getDecl());
  if (const auto *M = dyn_cast_or_null<MemberExpr>(E); M && !M->isArrow()) return namedVariable(M->getBase());
  return nullptr;
}
std::string objectKey(const Expr *E) {
  E = stripAll(E);
  if (const auto *D = dyn_cast_or_null<DeclRefExpr>(E)) {
    if (!isa<VarDecl>(D->getDecl())) return "";
    return D->getDecl()->getNameAsString() + "@" + std::to_string(reinterpret_cast<uintptr_t>(D->getDecl()));
  }
  if (const auto *M = dyn_cast_or_null<MemberExpr>(E)) {
    std::string Base = objectKey(M->getBase());
    if (Base.empty()) return "";
    return Base + (M->isArrow() ? "->" : ".") + M->getMemberDecl()->getNameAsString();
  }
  if (const auto *U = dyn_cast_or_null<UnaryOperator>(E); U && U->getOpcode() == UO_Deref) {
    std::string Base = objectKey(U->getSubExpr());
    return Base.empty() ? "" : "*" + Base;
  }
  return "";
}

std::optional<llvm::APSInt> integerConstant(const Expr *E, ASTContext &C) {
  // The value as written, before any implicit conversion of the context.
  E = stripAll(E);
  if (!E || E->containsErrors() || !E->getType()->isIntegralOrEnumerationType() || E->isValueDependent())
    return std::nullopt;
  return E->getIntegerConstantExpr(C);
}
static bool refersToObjectOrCall(const Stmt *S) {
  if (!S) return false;
  if (const auto *D = dyn_cast<DeclRefExpr>(S); D && isa<VarDecl>(D->getDecl())) return true;
  if (isa<CallExpr>(S)) return true;
  for (const Stmt *Child : S->children())
    if (refersToObjectOrCall(Child)) return true;
  return false;
}
bool isConstantExpression(const Expr *E, ASTContext &C) {
  if (!E || E->containsErrors()) return false;
  if (integerConstant(E, C)) return true;
  return !refersToObjectOrCall(E) && E->isEvaluatable(C);
}

bool isNoReturnCall(const Stmt *S) {
  const auto *E = dyn_cast_or_null<Expr>(S);
  if (!E) return false;
  const auto *Call = dyn_cast<CallExpr>(stripAll(E));
  if (!Call) return false;
  const auto *F = Call->getDirectCallee();
  return F && F->isNoReturn();
}
// Whether S contains a break (or continue) that leaves the enclosing loop.
static bool leavesLoop(const Stmt *S, bool Break, bool Continue) {
  if (!S) return false;
  if (Break && isa<BreakStmt>(S)) return true;
  if (Continue && isa<ContinueStmt>(S)) return true;
  if (isa<WhileStmt, DoStmt, ForStmt>(S)) return false;
  if (isa<SwitchStmt>(S)) Break = false;
  for (const Stmt *Child : S->children())
    if (leavesLoop(Child, Break, Continue)) return true;
  return false;
}
static bool loopBodyBreaks(const Stmt *Body) { return leavesLoop(Body, true, false); }
static std::optional<bool> constantCondition(const Expr *E, ASTContext &C) {
  bool Value = false;
  if (E && !E->containsErrors() && !E->HasSideEffects(C) && E->EvaluateAsBooleanCondition(Value, C)) return Value;
  return std::nullopt;
}
bool canCompleteNormally(const Stmt *S, ASTContext &C) {
  if (!S) return true;
  if (isa<ReturnStmt, GotoStmt, IndirectGotoStmt, BreakStmt, ContinueStmt>(S)) return false;
  if (const auto *Block = dyn_cast<CompoundStmt>(S)) {
    bool Reachable = true;
    for (const Stmt *Child : Block->body()) {
      if (isa<LabelStmt, SwitchCase>(Child)) Reachable = true;
      if (Reachable && !canCompleteNormally(Child, C)) Reachable = false;
    }
    return Reachable;
  }
  if (const auto *If = dyn_cast<IfStmt>(S)) {
    auto Known = constantCondition(If->getCond(), C);
    if (Known && *Known) return canCompleteNormally(If->getThen(), C);
    if (Known && !*Known) return !If->getElse() || canCompleteNormally(If->getElse(), C);
    if (!If->getElse()) return true;
    return canCompleteNormally(If->getThen(), C) || canCompleteNormally(If->getElse(), C);
  }
  if (const auto *W = dyn_cast<WhileStmt>(S)) {
    auto Known = constantCondition(W->getCond(), C);
    return !(Known && *Known) || loopBodyBreaks(W->getBody());
  }
  if (const auto *F = dyn_cast<ForStmt>(S)) {
    auto Known = F->getCond() ? constantCondition(F->getCond(), C) : std::optional<bool>(true);
    return !(Known && *Known) || loopBodyBreaks(F->getBody());
  }
  if (const auto *D = dyn_cast<DoStmt>(S)) {
    auto Known = constantCondition(D->getCond(), C);
    if (Known && *Known) return loopBodyBreaks(D->getBody());
    return canCompleteNormally(D->getBody(), C) || leavesLoop(D->getBody(), true, true);
  }
  if (const auto *Sw = dyn_cast<SwitchStmt>(S)) {
    bool HasDefault = false;
    for (const SwitchCase *Case = Sw->getSwitchCaseList(); Case; Case = Case->getNextSwitchCase())
      HasDefault |= isa<DefaultStmt>(Case);
    return !HasDefault || loopBodyBreaks(Sw->getBody()) || canCompleteNormally(Sw->getBody(), C);
  }
  if (const auto *L = dyn_cast<LabelStmt>(S)) return canCompleteNormally(L->getSubStmt(), C);
  if (const auto *Case = dyn_cast<SwitchCase>(S)) return canCompleteNormally(Case->getSubStmt(), C);
  return !isNoReturnCall(S);
}

std::string sourceText(const SourceManager &SM, const LangOptions &LO, SourceRange R) {
  return Lexer::getSourceText(CharSourceRange::getTokenRange(R), SM, LO).str();
}
bool rangeHasComment(const SourceManager &SM, const LangOptions &LO, SourceRange R) {
  std::string Text = sourceText(SM, LO, R);
  return Text.find("/*") != std::string::npos || Text.find("//") != std::string::npos;
}
bool expandedFromMacro(const SourceManager &SM, const LangOptions &LO, SourceLocation L, llvm::StringRef Name) {
  while (L.isMacroID()) {
    if (Lexer::getImmediateMacroName(L, SM, LO) == Name) return true;
    L = SM.getImmediateMacroCallerLoc(L);
  }
  return false;
}
std::string spelling(const SourceManager &SM, const LangOptions &LO, SourceLocation L) {
  llvm::SmallString<64> Buffer;
  bool Invalid = false;
  llvm::StringRef Text = Lexer::getSpelling(SM.getSpellingLoc(L), Buffer, SM, LO, &Invalid);
  return Invalid ? "" : Text.str();
}

namespace {
class CommentCallback final : public CommentHandler {
public:
  explicit CommentCallback(std::function<void(SourceRange, llvm::StringRef)> F) : F(std::move(F)) {}
  bool HandleComment(Preprocessor &PP, SourceRange Comment) override {
    const SourceManager &SM = PP.getSourceManager();
    if (!isUserLocation(SM, Comment.getBegin())) return false;
    bool Invalid = false;
    llvm::StringRef Text =
        Lexer::getSourceText(CharSourceRange::getCharRange(Comment), SM, PP.getLangOpts(), &Invalid);
    if (!Invalid) F(Comment, Text);
    return false;
  }

private:
  std::function<void(SourceRange, llvm::StringRef)> F;
};
} // namespace
std::unique_ptr<CommentHandler> onComments(Preprocessor &PP, std::function<void(SourceRange, llvm::StringRef)> F) {
  auto Handler = std::make_unique<CommentCallback>(std::move(F));
  PP.addCommentHandler(Handler.get());
  return Handler;
}

// ---- essential type model ------------------------------------------------
const char *categoryName(Cat C) {
  switch (C) {
  case Cat::Boolean: return "Boolean";
  case Cat::Character: return "character";
  case Cat::Signed: return "signed";
  case Cat::Unsigned: return "unsigned";
  case Cat::Enum: return "enum";
  case Cat::Floating: return "floating";
  case Cat::None: break;
  }
  return "non-arithmetic";
}
static bool namedEnum(const EnumDecl *D) { return D->getIdentifier() || D->getTypedefNameForAnonDecl(); }

EType essentialOfType(QualType T, ASTContext &C) {
  if (T.isNull()) return {};
  T = T.getCanonicalType();
  if (const auto *E = T->getAs<EnumType>()) {
    const EnumDecl *D = E->getDecl();
    if (namedEnum(D)) return {Cat::Enum, C.getTypeSize(T), D};
    return {Cat::Signed, C.getTypeSize(C.IntTy)};
  }
  const auto *B = T->getAs<BuiltinType>();
  if (!B) return {};
  uint64_t Bits = C.getTypeSize(T);
  switch (B->getKind()) {
  case BuiltinType::Bool: return {Cat::Boolean, Bits};
  case BuiltinType::Char_S:
  case BuiltinType::Char_U: return {Cat::Character, Bits};
  case BuiltinType::SChar:
  case BuiltinType::Short:
  case BuiltinType::Int:
  case BuiltinType::Long:
  case BuiltinType::LongLong:
  case BuiltinType::Int128: return {Cat::Signed, Bits};
  case BuiltinType::UChar:
  case BuiltinType::UShort:
  case BuiltinType::UInt:
  case BuiltinType::ULong:
  case BuiltinType::ULongLong:
  case BuiltinType::UInt128: return {Cat::Unsigned, Bits};
  case BuiltinType::Half:
  case BuiltinType::Float16:
  case BuiltinType::Float:
  case BuiltinType::Double:
  case BuiltinType::LongDouble:
  case BuiltinType::Float128: return {Cat::Floating, Bits};
  default: return {};
  }
}
static EType lowestRank(const llvm::APSInt &V, bool Signed, ASTContext &C) {
  llvm::APSInt X = V.extend(130);
  X.setIsSigned(true);
  QualType Types[] = {Signed ? C.SignedCharTy : C.UnsignedCharTy, Signed ? C.ShortTy : C.UnsignedShortTy,
                      Signed ? C.IntTy : C.UnsignedIntTy, Signed ? C.LongTy : C.UnsignedLongTy,
                      Signed ? C.LongLongTy : C.UnsignedLongLongTy};
  for (QualType T : Types) {
    unsigned W = C.getIntWidth(T);
    bool Fits = Signed ? X.getSignificantBits() <= W : (!X.isNegative() && X.getActiveBits() <= W);
    if (Fits) return {Signed ? Cat::Signed : Cat::Unsigned, W};
  }
  return {Signed ? Cat::Signed : Cat::Unsigned, C.getIntWidth(C.LongLongTy)};
}
static EType widthType(uint64_t Width, bool Signed, ASTContext &C) {
  for (QualType T : {Signed ? C.SignedCharTy : C.UnsignedCharTy, Signed ? C.ShortTy : C.UnsignedShortTy,
                     Signed ? C.IntTy : C.UnsignedIntTy, Signed ? C.LongTy : C.UnsignedLongTy})
    if (C.getIntWidth(T) >= Width) return {Signed ? Cat::Signed : Cat::Unsigned, C.getIntWidth(T)};
  return {Signed ? Cat::Signed : Cat::Unsigned, 64};
}
static EType wider(const EType &A, const EType &B) { return A.Bits >= B.Bits ? A : B; }
static bool integerCategory(const EType &T) { return T.C == Cat::Signed || T.C == Cat::Unsigned; }

static EType binaryEssential(const BinaryOperator *B, ASTContext &C) {
  BinaryOperatorKind Op = B->getOpcode();
  if (B->isComparisonOp() || B->isLogicalOp()) return {Cat::Boolean, C.getTypeSize(C.BoolTy)};
  if (Op == BO_Comma) return essentialType(B->getRHS(), C);
  if (B->isAssignmentOp()) return essentialType(B->getLHS(), C);
  EType L = essentialType(B->getLHS(), C), R = essentialType(B->getRHS(), C);
  EType Standard = essentialOfType(B->getType(), C);
  if (B->isShiftOp()) {
    if (L.C != Cat::Unsigned) return Standard;
    if (auto V = integerConstant(B, C)) return lowestRank(*V, false, C);
    return L;
  }
  if (Op == BO_Add && ((L.C == Cat::Character && integerCategory(R)) || (R.C == Cat::Character && integerCategory(L))))
    return {Cat::Character, C.getTypeSize(C.CharTy)};
  if (Op == BO_Sub && L.C == Cat::Character && integerCategory(R)) return {Cat::Character, C.getTypeSize(C.CharTy)};
  if (B->isMultiplicativeOp() || B->isAdditiveOp() || B->isBitwiseOp()) {
    if (L.C == Cat::Signed && R.C == Cat::Signed) {
      if (auto V = integerConstant(B, C)) return lowestRank(*V, true, C);
      return wider(L, R);
    }
    if (L.C == Cat::Unsigned && R.C == Cat::Unsigned) {
      if (auto V = integerConstant(B, C)) return lowestRank(*V, false, C);
      return wider(L, R);
    }
  }
  return Standard;
}

EType essentialType(const Expr *E, ASTContext &C) {
  if (!E || E->containsErrors()) return {};
  E = E->IgnoreParenImpCasts();
  const SourceManager &SM = C.getSourceManager();
  const LangOptions &LO = C.getLangOpts();
  if (const auto *I = dyn_cast<IntegerLiteral>(E)) {
    if (I->getBeginLoc().isMacroID() &&
        (expandedFromMacro(SM, LO, I->getBeginLoc(), "true") || expandedFromMacro(SM, LO, I->getBeginLoc(), "false")))
      return {Cat::Boolean, C.getTypeSize(C.BoolTy)};
    llvm::APSInt V(I->getValue(), I->getType()->isUnsignedIntegerType());
    QualType T = I->getType().getCanonicalType();
    if (T == C.IntTy) return lowestRank(V, true, C);
    if (T == C.UnsignedIntTy) return lowestRank(V, false, C);
    return essentialOfType(T, C);
  }
  if (const auto *Ch = dyn_cast<CharacterLiteral>(E)) {
    if ((Ch->getKind() == CharacterLiteralKind::Ascii || Ch->getKind() == CharacterLiteralKind::UTF8) &&
        Ch->getValue() <= 0xFF)
      return {Cat::Character, C.getTypeSize(C.CharTy)};
    return essentialOfType(Ch->getType(), C);
  }
  if (const auto *D = dyn_cast<DeclRefExpr>(E)) {
    if (const auto *Constant = dyn_cast<EnumConstantDecl>(D->getDecl())) {
      const auto *Enum = cast<EnumDecl>(Constant->getDeclContext());
      if (namedEnum(Enum)) return {Cat::Enum, C.getTypeSize(C.getTypeDeclType(Enum)), Enum};
      return lowestRank(Constant->getInitVal(), true, C);
    }
  }
  if (const auto *M = dyn_cast<MemberExpr>(E)) {
    if (const auto *F = dyn_cast<FieldDecl>(M->getMemberDecl()); F && F->isBitField()) {
      if (F->getType()->isBooleanType()) return {Cat::Boolean, C.getTypeSize(C.BoolTy)};
      if (F->getType()->isEnumeralType()) return essentialOfType(F->getType(), C);
      return widthType(F->getBitWidthValue(C), F->getType()->isSignedIntegerType(), C);
    }
  }
  if (const auto *B = dyn_cast<BinaryOperator>(E)) return binaryEssential(B, C);
  if (const auto *U = dyn_cast<UnaryOperator>(E)) {
    EType Operand = essentialType(U->getSubExpr(), C);
    EType Standard = essentialOfType(U->getType(), C);
    switch (U->getOpcode()) {
    case UO_LNot: return {Cat::Boolean, C.getTypeSize(C.BoolTy)};
    case UO_Minus:
      if (Operand.C != Cat::Signed) return Standard;
      if (auto V = integerConstant(U, C)) return lowestRank(*V, true, C);
      return Operand;
    case UO_Plus: return integerCategory(Operand) ? Operand : Standard;
    case UO_Not:
      if (Operand.C != Cat::Unsigned) return Standard;
      if (auto V = integerConstant(U, C)) return lowestRank(*V, false, C);
      return Operand;
    case UO_PreInc:
    case UO_PreDec:
    case UO_PostInc:
    case UO_PostDec: return Operand;
    default: return Standard;
    }
  }
  if (const auto *Cond = dyn_cast<ConditionalOperator>(E)) {
    EType A = essentialType(Cond->getTrueExpr(), C), B = essentialType(Cond->getFalseExpr(), C);
    if (A.same(B) && A.Bits == B.Bits) return A;
    if ((A.C == Cat::Signed && B.C == Cat::Signed) || (A.C == Cat::Unsigned && B.C == Cat::Unsigned)) return wider(A, B);
    return essentialOfType(Cond->getType(), C);
  }
  return essentialOfType(E->getType(), C);
}

bool isComposite(const Expr *E, ASTContext &C) {
  E = stripAll(E);
  if (const auto *B = dyn_cast_or_null<BinaryOperator>(E)) {
    if (!(B->isMultiplicativeOp() || B->isAdditiveOp() || B->isBitwiseOp() || B->isShiftOp())) return false;
    return B->getType()->isArithmeticType() && !isConstantExpression(B, C);
  }
  if (const auto *Cond = dyn_cast_or_null<ConditionalOperator>(E))
    return (isComposite(Cond->getTrueExpr(), C) || isComposite(Cond->getFalseExpr(), C)) &&
           !isConstantExpression(Cond, C);
  return false;
}

namespace {
class AssignmentVisitor : public RecursiveASTVisitor<AssignmentVisitor> {
public:
  AssignmentVisitor(ASTContext &C, std::function<void(const Assignment &)> F) : C(C), F(std::move(F)) {}
  bool TraverseFunctionDecl(FunctionDecl *D) {
    const FunctionDecl *Saved = Current;
    Current = D;
    bool Continue = RecursiveASTVisitor::TraverseFunctionDecl(D);
    Current = Saved;
    return Continue;
  }
  bool user(SourceLocation L) { return isUserLocation(C.getSourceManager(), L); }
  void emit(const Expr *Source, EType Target, SourceLocation Loc, bool Case = false) {
    if (Source && Target.arithmetic() && user(Loc)) F({Source, Target, Loc, Case});
  }
  bool VisitBinaryOperator(BinaryOperator *B) {
    if (B->getOpcode() == BO_Assign) emit(B->getRHS(), essentialType(B->getLHS(), C), B->getOperatorLoc());
    return true;
  }
  bool VisitVarDecl(VarDecl *D) {
    if (D->hasInit() && !isa<InitListExpr>(D->getInit()->IgnoreImplicit()) && !isa<ParmVarDecl>(D))
      emit(D->getInit(), essentialOfType(D->getType(), C), D->getInit()->getBeginLoc());
    return true;
  }
  static bool zeroList(const InitListExpr *L) {
    const InitListExpr *Syntax = L->isSemanticForm() && L->getSyntacticForm() ? L->getSyntacticForm() : L;
    if (Syntax->getNumInits() != 1) return false;
    const auto *I = dyn_cast<IntegerLiteral>(stripAll(Syntax->getInit(0)));
    return I && I->getValue() == 0;
  }
  bool VisitInitListExpr(InitListExpr *L) {
    if (!L->isSemanticForm() || Zero(L)) return true;
    for (const Expr *Init : L->inits()) {
      if (!Init || isa<InitListExpr, ImplicitValueInitExpr, DesignatedInitUpdateExpr>(Init)) continue;
      emit(Init, essentialOfType(Init->getType(), C), Init->getBeginLoc());
    }
    return true;
  }
  bool Zero(const InitListExpr *L) {
    for (const Stmt *S = L; S; S = parentStmt(C, S)) {
      const auto *List = dyn_cast<InitListExpr>(S);
      if (!List) break;
      if (zeroList(List)) return true;
    }
    return false;
  }
  bool VisitCallExpr(CallExpr *Call) {
    QualType T = Call->getCallee()->getType();
    if (const auto *P = T->getAs<PointerType>()) T = P->getPointeeType();
    const auto *Proto = T->getAs<FunctionProtoType>();
    if (!Proto) return true;
    for (unsigned I = 0; I < Call->getNumArgs() && I < Proto->getNumParams(); ++I)
      emit(Call->getArg(I), essentialOfType(Proto->getParamType(I), C), Call->getArg(I)->getBeginLoc());
    return true;
  }
  bool VisitReturnStmt(ReturnStmt *R) {
    if (Current && R->getRetValue()) emit(R->getRetValue(), essentialOfType(Current->getReturnType(), C), R->getBeginLoc());
    return true;
  }
  bool VisitSwitchStmt(SwitchStmt *S) {
    EType Control = essentialType(S->getCond(), C);
    for (const SwitchCase *Case = S->getSwitchCaseList(); Case; Case = Case->getNextSwitchCase())
      if (const auto *Label = dyn_cast<CaseStmt>(Case)) emit(Label->getLHS(), Control, Label->getLHS()->getBeginLoc(), true);
    return true;
  }

private:
  ASTContext &C;
  std::function<void(const Assignment &)> F;
  const FunctionDecl *Current = nullptr;
};
} // namespace
void forEachAssignment(ASTContext &C, std::function<void(const Assignment &)> F) {
  AssignmentVisitor V(C, std::move(F));
  V.TraverseDecl(C.getTranslationUnitDecl());
}

// ---- library identifiers ---------------------------------------------------
static const char *const C90Names =
    "assert NDEBUG isalnum isalpha iscntrl isdigit isgraph islower isprint ispunct isspace isupper isxdigit "
    "tolower toupper errno EDOM ERANGE EILSEQ FLT_RADIX FLT_ROUNDS FLT_MANT_DIG DBL_MANT_DIG LDBL_MANT_DIG "
    "FLT_DIG DBL_DIG LDBL_DIG FLT_MIN_EXP DBL_MIN_EXP LDBL_MIN_EXP FLT_MIN_10_EXP DBL_MIN_10_EXP "
    "LDBL_MIN_10_EXP FLT_MAX_EXP DBL_MAX_EXP LDBL_MAX_EXP FLT_MAX_10_EXP DBL_MAX_10_EXP LDBL_MAX_10_EXP "
    "FLT_MAX DBL_MAX LDBL_MAX FLT_EPSILON DBL_EPSILON LDBL_EPSILON FLT_MIN DBL_MIN LDBL_MIN CHAR_BIT "
    "SCHAR_MIN SCHAR_MAX UCHAR_MAX CHAR_MIN CHAR_MAX MB_LEN_MAX SHRT_MIN SHRT_MAX USHRT_MAX INT_MIN "
    "INT_MAX UINT_MAX LONG_MIN LONG_MAX ULONG_MAX lconv setlocale localeconv LC_ALL LC_COLLATE LC_CTYPE "
    "LC_MONETARY LC_NUMERIC LC_TIME HUGE_VAL jmp_buf setjmp longjmp sig_atomic_t signal raise SIG_DFL "
    "SIG_ERR SIG_IGN SIGABRT SIGFPE SIGILL SIGINT SIGSEGV SIGTERM va_list va_start va_arg va_end ptrdiff_t "
    "size_t wchar_t NULL offsetof FILE fpos_t _IOFBF _IOLBF _IONBF BUFSIZ EOF FOPEN_MAX FILENAME_MAX "
    "L_tmpnam SEEK_CUR SEEK_END SEEK_SET TMP_MAX stderr stdin stdout remove rename tmpfile tmpnam fclose "
    "fflush fopen freopen setbuf setvbuf fprintf fscanf printf scanf sprintf sscanf vfprintf vprintf "
    "vsprintf fgetc fgets fputc fputs getc getchar gets putc putchar puts ungetc fread fwrite fgetpos "
    "fseek fsetpos ftell rewind clearerr feof ferror perror div_t ldiv_t EXIT_FAILURE EXIT_SUCCESS "
    "RAND_MAX MB_CUR_MAX atof atoi atol strtod strtol strtoul rand srand calloc free malloc realloc abort "
    "atexit exit getenv system bsearch qsort abs labs div ldiv mblen mbtowc wctomb mbstowcs wcstombs "
    "memcpy memmove strcpy strncpy strcat strncat memcmp strcmp strcoll strncmp strxfrm memchr strchr "
    "strcspn strpbrk strrchr strspn strstr strtok memset strerror strlen clock_t time_t tm CLOCKS_PER_SEC "
    "clock difftime mktime time asctime ctime gmtime localtime strftime defined";
static const char *const C99Names =
    "isblank fenv_t fexcept_t feclearexcept fegetexceptflag feraiseexcept fesetexceptflag fetestexcept "
    "fegetround fesetround fegetenv feholdexcept fesetenv feupdateenv FE_DIVBYZERO FE_INEXACT FE_INVALID "
    "FE_OVERFLOW FE_UNDERFLOW FE_ALL_EXCEPT FE_DOWNWARD FE_TONEAREST FE_TOWARDZERO FE_UPWARD FE_DFL_ENV "
    "FLT_EVAL_METHOD DECIMAL_DIG imaxdiv_t imaxabs imaxdiv strtoimax strtoumax wcstoimax wcstoumax and "
    "and_eq bitand bitor compl not not_eq or or_eq xor xor_eq LLONG_MIN LLONG_MAX ULLONG_MAX HUGE_VALF "
    "HUGE_VALL INFINITY NAN FP_INFINITE FP_NAN FP_NORMAL FP_SUBNORMAL FP_ZERO FP_FAST_FMA FP_FAST_FMAF "
    "FP_FAST_FMAL FP_ILOGB0 FP_ILOGBNAN MATH_ERRNO MATH_ERREXCEPT math_errhandling fpclassify isfinite "
    "isinf isnan isnormal signbit isgreater isgreaterequal isless islessequal islessgreater isunordered "
    "float_t double_t va_copy bool true false __bool_true_false_are_defined intptr_t uintptr_t intmax_t "
    "uintmax_t INTPTR_MIN INTPTR_MAX UINTPTR_MAX INTMAX_MIN INTMAX_MAX UINTMAX_MAX PTRDIFF_MIN PTRDIFF_MAX "
    "SIG_ATOMIC_MIN SIG_ATOMIC_MAX SIZE_MAX WCHAR_MIN WCHAR_MAX WINT_MIN WINT_MAX INTMAX_C UINTMAX_C "
    "snprintf vfscanf vscanf vsnprintf vsscanf lldiv_t atoll strtof strtold strtoll strtoull _Exit llabs "
    "lldiv mbstate_t wint_t WEOF fwprintf fwscanf swprintf swscanf vfwprintf vfwscanf vswprintf vswscanf "
    "vwprintf vwscanf wprintf wscanf fgetwc fgetws fputwc fputws fwide getwc getwchar putwc putwchar "
    "ungetwc wcstod wcstof wcstold wcstol wcstoll wcstoul wcstoull wcscpy wcsncpy wmemcpy wmemmove wcscat "
    "wcsncat wcscmp wcscoll wcsncmp wcsxfrm wmemcmp wcschr wcscspn wcspbrk wcsrchr wcsspn wcsstr wcstok "
    "wmemchr wcslen wmemset wcsftime btowc wctob mbsinit mbrlen mbrtowc wcrtomb mbsrtowcs wcsrtombs "
    "wctrans_t wctype_t iswalnum iswalpha iswblank iswcntrl iswdigit iswgraph iswlower iswprint iswpunct "
    "iswspace iswupper iswxdigit iswctype wctype towlower towupper towctrans wctrans complex _Complex_I "
    "imaginary _Imaginary_I I";
static const char *const MathNames =
    "acos asin atan atan2 cos sin tan acosh asinh atanh cosh sinh tanh exp exp2 expm1 frexp ilogb ldexp "
    "log log10 log1p log2 logb modf scalbn scalbln cbrt fabs hypot pow sqrt erf erfc lgamma tgamma ceil "
    "floor nearbyint rint lrint llrint round lround llround trunc fmod remainder remquo copysign nan "
    "nextafter nexttoward fdim fmax fmin fma cacos casin catan ccos csin ctan cacosh casinh catanh ccosh "
    "csinh ctanh cexp clog cabs cpow csqrt carg cimag conj cproj creal";

static llvm::StringSet<> makeSet(bool C99) {
  llvm::StringSet<> Set;
  auto Add = [&Set](llvm::StringRef List, llvm::StringRef Suffix) {
    llvm::SmallVector<llvm::StringRef, 256> Parts;
    List.split(Parts, ' ', -1, false);
    for (llvm::StringRef P : Parts) Set.insert((P + Suffix).str());
  };
  Add(C90Names, "");
  Add(MathNames, "");
  if (C99) {
    Add(C99Names, "");
    Add(MathNames, "f");
    Add(MathNames, "l");
    for (const char *Width : {"8", "16", "32", "64"}) {
      std::string W = Width;
      for (const char *Kind : {"int", "uint", "int_least", "uint_least", "int_fast", "uint_fast"})
        Set.insert(std::string(Kind) + W + "_t");
      for (const char *Macro : {"INT", "UINT", "INT_LEAST", "UINT_LEAST", "INT_FAST", "UINT_FAST"}) {
        Set.insert(std::string(Macro) + W + "_MAX");
        if (Macro[0] == 'I') Set.insert(std::string(Macro) + W + "_MIN");
      }
      Set.insert("INT" + W + "_C");
      Set.insert("UINT" + W + "_C");
    }
  }
  return Set;
}
static bool formatMacro(llvm::StringRef Name) {
  if (!Name.consume_front("PRI") && !Name.consume_front("SCN")) return false;
  return !Name.empty() && llvm::StringRef("diouxX").contains(Name.front());
}
bool isLibraryIdentifier(llvm::StringRef Name, bool C99) {
  static const llvm::StringSet<> C90Set = makeSet(false), C99Set = makeSet(true);
  return C99 ? (C99Set.contains(Name) || formatMacro(Name)) : C90Set.contains(Name);
}
} // namespace safety::misra
