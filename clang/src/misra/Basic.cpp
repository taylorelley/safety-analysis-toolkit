// SPDX-License-Identifier: Apache-2.0
// Rules 1.x-4.x: standard environment, unused code, comments and lexical
// conventions.
#include "Misra.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Lex/Lexer.h"
#include "clang/Lex/MacroInfo.h"
#include <map>
#include <set>

using namespace clang;
using namespace clang::ast_matchers;

namespace safety::misra {
namespace {
// ---- Rule 1.1: translation limits (compiler errors are reported by Clang) --
struct Limits {
  unsigned Params, Args, Members, Enumerators, Blocks, Includes, Conditionals, String, MacroParams;
};
Limits limitsFor(const LangOptions &LO) {
  if (LO.C99) return {127, 127, 1023, 1023, 127, 15, 63, 4095, 127};
  return {31, 31, 127, 127, 15, 8, 8, 509, 31};
}
class LimitVisitor : public RecursiveASTVisitor<LimitVisitor> {
public:
  LimitVisitor(Rule &R, ASTContext &C) : R(R), C(C), L(limitsFor(C.getLangOpts())) {}
  bool user(SourceLocation Loc) { return isUserLocation(C.getSourceManager(), Loc); }
  void exceed(SourceLocation Loc, const char *What, unsigned Limit) {
    if (user(Loc)) R.report(Loc, (llvm::Twine(What) + " exceeds the minimum translation limit of " + llvm::Twine(Limit)).str());
  }
  bool VisitFunctionDecl(FunctionDecl *F) {
    if (F->getNumParams() > L.Params) exceed(F->getLocation(), "number of parameters", L.Params);
    return true;
  }
  bool VisitCallExpr(CallExpr *E) {
    if (E->getNumArgs() > L.Args) exceed(E->getBeginLoc(), "number of arguments", L.Args);
    return true;
  }
  bool VisitRecordDecl(RecordDecl *D) {
    if (D->isThisDeclarationADefinition() && std::distance(D->field_begin(), D->field_end()) > L.Members)
      exceed(D->getLocation(), "number of members", L.Members);
    return true;
  }
  bool VisitEnumDecl(EnumDecl *D) {
    if (std::distance(D->enumerator_begin(), D->enumerator_end()) > L.Enumerators)
      exceed(D->getLocation(), "number of enumeration constants", L.Enumerators);
    return true;
  }
  bool VisitStringLiteral(StringLiteral *S) {
    if (S->getLength() > L.String) exceed(S->getBeginLoc(), "string literal length", L.String);
    return true;
  }
  bool TraverseCompoundStmt(CompoundStmt *S) {
    if (++Depth > L.Blocks) exceed(S->getLBracLoc(), "block nesting depth", L.Blocks);
    bool Continue = RecursiveASTVisitor::TraverseCompoundStmt(S);
    --Depth;
    return Continue;
  }

private:
  Rule &R;
  ASTContext &C;
  Limits L;
  unsigned Depth = 0;
};
class LimitCallbacks final : public PPCallbacks {
public:
  LimitCallbacks(Rule &R, Preprocessor &PP) : R(R), PP(PP), L(limitsFor(PP.getLangOpts())) {}
  void exceed(SourceLocation Loc, const char *What, unsigned Limit) {
    if (isUserLocation(PP.getSourceManager(), Loc))
      R.report(Loc, (llvm::Twine(What) + " exceeds the minimum translation limit of " + llvm::Twine(Limit)).str());
  }
  void FileChanged(SourceLocation Loc, FileChangeReason Reason, SrcMgr::CharacteristicKind, FileID) override {
    if (Reason == EnterFile && ++IncludeDepth > L.Includes + 1) exceed(Loc, "#include nesting depth", L.Includes);
    if (Reason == ExitFile && IncludeDepth > 0) --IncludeDepth;
  }
  void push(SourceLocation Loc) {
    if (++Conditionals > L.Conditionals) exceed(Loc, "conditional inclusion nesting depth", L.Conditionals);
  }
  void If(SourceLocation Loc, SourceRange, ConditionValueKind) override { push(Loc); }
  void Ifdef(SourceLocation Loc, const Token &, const MacroDefinition &) override { push(Loc); }
  void Ifndef(SourceLocation Loc, const Token &, const MacroDefinition &) override { push(Loc); }
  void Endif(SourceLocation, SourceLocation) override {
    if (Conditionals > 0) --Conditionals;
  }
  void MacroDefined(const Token &Name, const MacroDirective *MD) override {
    if (MD->getMacroInfo()->getNumParams() > L.MacroParams)
      exceed(Name.getLocation(), "number of macro parameters", L.MacroParams);
  }

private:
  Rule &R;
  Preprocessor &PP;
  Limits L;
  unsigned IncludeDepth = 0, Conditionals = 0;
};
Spec rule11() {
  Spec S = unitSpec([](Rule &R, ASTContext &C) { LimitVisitor(R, C).TraverseDecl(C.getTranslationUnitDecl()); });
  S.Preprocessor = [](Rule &R, Preprocessor &PP, const SourceManager &) {
    PP.addPPCallbacks(std::make_unique<LimitCallbacks>(R, PP));
  };
  return S;
}

// ---- Rule 1.2: language extensions --------------------------------------
class ExtensionVisitor : public RecursiveASTVisitor<ExtensionVisitor> {
public:
  ExtensionVisitor(Rule &R, ASTContext &C) : R(R), C(C) {}
  void flag(SourceLocation Loc, const char *What) {
    if (isUserLocation(C.getSourceManager(), Loc)) R.report(Loc, (llvm::Twine("language extension used: ") + What).str());
  }
  bool VisitStmtExpr(StmtExpr *E) { return flag(E->getBeginLoc(), "statement expression"), true; }
  bool VisitBinaryConditionalOperator(BinaryConditionalOperator *E) {
    return flag(E->getBeginLoc(), "conditional with omitted operand"), true;
  }
  bool VisitCaseStmt(CaseStmt *S) {
    if (S->caseStmtIsGNURange()) flag(S->getBeginLoc(), "case range");
    return true;
  }
  bool VisitAddrLabelExpr(AddrLabelExpr *E) { return flag(E->getBeginLoc(), "label address"), true; }
  bool VisitIndirectGotoStmt(IndirectGotoStmt *S) { return flag(S->getBeginLoc(), "computed goto"), true; }
  bool VisitAsmStmt(AsmStmt *S) { return flag(S->getAsmLoc(), "inline assembly"), true; }
  bool VisitFileScopeAsmDecl(FileScopeAsmDecl *D) { return flag(D->getAsmLoc(), "assembly declaration"), true; }
  bool VisitUnaryOperator(UnaryOperator *U) {
    if (U->getOpcode() == UO_Extension) flag(U->getOperatorLoc(), "__extension__");
    return true;
  }
  bool VisitDesignatedInitExpr(DesignatedInitExpr *E) {
    for (const auto &D : E->designators())
      if (D.isArrayRangeDesignator()) flag(D.getBeginLoc(), "array range designator");
    return true;
  }
  bool VisitFieldDecl(FieldDecl *F) {
    if (const auto *A = C.getAsConstantArrayType(F->getType()); A && A->getSize() == 0)
      flag(F->getLocation(), "zero-length array");
    return true;
  }
  bool VisitDecl(Decl *D) {
    for (const Attr *A : D->attrs())
      if (!A->isImplicit() && A->getLocation().isValid() &&
          (A->getSyntax() == AttributeCommonInfo::AS_GNU || A->getSyntax() == AttributeCommonInfo::AS_Declspec))
        flag(A->getLocation(), "attribute");
    return true;
  }
  bool VisitTypeOfExprTypeLoc(TypeOfExprTypeLoc TL) {
    if (!C.getLangOpts().C23) flag(TL.getBeginLoc(), "typeof");
    return true;
  }
  bool VisitTypeOfTypeLoc(TypeOfTypeLoc TL) {
    if (!C.getLangOpts().C23) flag(TL.getBeginLoc(), "typeof");
    return true;
  }
  bool VisitBuiltinTypeLoc(BuiltinTypeLoc TL) {
    auto Kind = TL.getTypePtr()->getKind();
    if (Kind == BuiltinType::Int128 || Kind == BuiltinType::UInt128 || Kind == BuiltinType::Float128)
      flag(TL.getBeginLoc(), "extended builtin type");
    return true;
  }

private:
  Rule &R;
  ASTContext &C;
};

// ---- Rule 2.1 / 2.2 ------------------------------------------------------
void unreachable(Rule &R, ASTContext &C, const CompoundStmt *Block) {
  bool Reachable = true;
  for (const Stmt *Child : Block->body()) {
    if (isa<LabelStmt, SwitchCase>(Child)) Reachable = true;
    if (!Reachable && !isa<NullStmt>(Child)) {
      if (isUserLocation(C.getSourceManager(), Child->getBeginLoc()))
        R.report(Child->getBeginLoc(), "statement can never be executed");
      return;
    }
    if (!canCompleteNormally(Child, C)) Reachable = false;
  }
}
bool effectFree(const Expr *E) {
  if (isVolatileRead(E)) return false;
  E = E->IgnoreParens();
  if (const auto *Cast = dyn_cast<ImplicitCastExpr>(E)) return effectFree(Cast->getSubExpr());
  if (const auto *B = dyn_cast<BinaryOperator>(E)) {
    if (B->getOpcode() == BO_Comma) return effectFree(B->getLHS()) && effectFree(B->getRHS());
    if (B->isAssignmentOp()) return false;
  }
  if (const auto *U = dyn_cast<UnaryOperator>(E); U && (U->isIncrementDecrementOp() || U->getOpcode() == UO_Extension))
    return false;
  if (isa<CallExpr, StmtExpr>(E)) return false;
  if (const auto *Cast = dyn_cast<CStyleCastExpr>(E); Cast && Cast->getType()->isVoidType()) return false;
  if ((isa<DeclRefExpr, MemberExpr>(E)) && E->getType().isVolatileQualified()) return false;
  return true;
}
class DeadCode : public RecursiveASTVisitor<DeadCode> {
public:
  DeadCode(Rule &R, ASTContext &C) : R(R), C(C) {}
  void statement(const Stmt *S) {
    const auto *E = dyn_cast_or_null<Expr>(S);
    if (!E || !isUserLocation(C.getSourceManager(), E->getBeginLoc())) return;
    if (effectFree(E)) R.report(E->getBeginLoc(), "operation has no effect and could be removed");
  }
  bool VisitCompoundStmt(CompoundStmt *S) {
    for (const Stmt *Child : S->body()) statement(Child);
    return true;
  }
  bool VisitIfStmt(IfStmt *S) { return statement(S->getThen()), statement(S->getElse()), true; }
  bool VisitWhileStmt(WhileStmt *S) { return statement(S->getBody()), true; }
  bool VisitDoStmt(DoStmt *S) { return statement(S->getBody()), true; }
  bool VisitForStmt(ForStmt *S) { return statement(S->getInit()), statement(S->getInc()), statement(S->getBody()), true; }
  bool VisitLabelStmt(LabelStmt *S) { return statement(S->getSubStmt()), true; }
  bool VisitCaseStmt(CaseStmt *S) { return statement(S->getSubStmt()), true; }
  bool VisitDefaultStmt(DefaultStmt *S) { return statement(S->getSubStmt()), true; }

private:
  Rule &R;
  ASTContext &C;
};

// ---- Rule 2.3 / 2.4 ------------------------------------------------------
class TagUses : public RecursiveASTVisitor<TagUses> {
public:
  explicit TagUses(const SourceManager &SM) : SM(SM) {}
  bool VisitElaboratedTypeLoc(ElaboratedTypeLoc TL) {
    // Typedef names are also wrapped in elaborated types; only tags count.
    auto Named = TL.getNamedTypeLoc().getAs<TagTypeLoc>();
    if (!Named) return true;
    const TagDecl *Tag = Named.getDecl();
    // A specifier that carries the tag's own definition is not a use of it.
    for (const TagDecl *D : Tag->redecls()) {
      if (D->isThisDeclarationADefinition() && D->getBeginLoc() == TL.getBeginLoc()) return true;
    }
    Used.insert(Tag->getCanonicalDecl());
    return true;
  }
  const SourceManager &SM;
  std::set<const Decl *> Used;
};

// ---- Rule 2.5 -------------------------------------------------------------
class MacroUses final : public PPCallbacks {
public:
  MacroUses(Rule &R, const SourceManager &SM) : R(R), SM(SM) {}
  void MacroDefined(const Token &Name, const MacroDirective *MD) override {
    if (SM.isInMainFile(Name.getLocation())) Definitions[MD->getMacroInfo()] = Name.getLocation();
  }
  void use(const MacroDefinition &MD) {
    if (const MacroInfo *MI = MD.getMacroInfo()) Used.insert(MI);
  }
  void MacroExpands(const Token &, const MacroDefinition &MD, SourceRange, const MacroArgs *) override { use(MD); }
  void Defined(const Token &, const MacroDefinition &MD, SourceRange) override { use(MD); }
  void Ifdef(SourceLocation, const Token &, const MacroDefinition &MD) override { use(MD); }
  void Ifndef(SourceLocation, const Token &, const MacroDefinition &MD) override { use(MD); }
  void Elifdef(SourceLocation, const Token &, const MacroDefinition &MD) override { use(MD); }
  void Elifndef(SourceLocation, const Token &, const MacroDefinition &MD) override { use(MD); }
  void MacroUndefined(const Token &, const MacroDefinition &MD, const MacroDirective *) override { use(MD); }
  void EndOfMainFile() override {
    for (const auto &[MI, Loc] : Definitions)
      if (!Used.count(MI)) R.report(Loc, "macro is defined but never used");
  }

private:
  Rule &R;
  const SourceManager &SM;
  std::map<const MacroInfo *, SourceLocation> Definitions;
  std::set<const MacroInfo *> Used;
};

// ---- Rule 2.6 -------------------------------------------------------------
class LabelUses : public RecursiveASTVisitor<LabelUses> {
public:
  bool VisitGotoStmt(GotoStmt *S) { return Used.insert(S->getLabel()), true; }
  bool VisitAddrLabelExpr(AddrLabelExpr *E) { return Used.insert(E->getLabel()), true; }
  bool VisitLabelStmt(LabelStmt *S) { return Labels.push_back(S->getDecl()), true; }
  std::set<const LabelDecl *> Used;
  std::vector<const LabelDecl *> Labels;
};

// ---- Rule 4.1 -------------------------------------------------------------
// Returns the offset of the first unterminated octal/hex escape in Text.
std::optional<size_t> unterminatedEscape(llvm::StringRef Text) {
  size_t Quote = Text.find_first_of("\"'");
  if (Quote == llvm::StringRef::npos) return std::nullopt;
  char Delim = Text[Quote];
  size_t End = Text.rfind(Delim);
  for (size_t I = Quote + 1; I < End; ++I) {
    if (Text[I] != '\\') continue;
    size_t Start = I;
    size_t J = I + 1;
    if (J < End && Text[J] >= '0' && Text[J] <= '7') {
      while (J < End && J < I + 4 && Text[J] >= '0' && Text[J] <= '7') ++J;
    } else if (J < End && Text[J] == 'x') {
      ++J;
      while (J < End && llvm::isHexDigit(Text[J])) ++J;
    } else {
      I = J;
      continue;
    }
    if (J < End && Text[J] != '\\') return Start;
    I = J - 1;
  }
  return std::nullopt;
}

// ---- Rule 4.2 -------------------------------------------------------------
class Trigraphs final : public PPCallbacks {
public:
  Trigraphs(Rule &R, const SourceManager &SM) : R(R), SM(SM) {}
  void FileChanged(SourceLocation Loc, FileChangeReason Reason, SrcMgr::CharacteristicKind Kind, FileID) override {
    if (Reason != EnterFile || Kind != SrcMgr::C_User || Loc.isInvalid()) return;
    FileID Id = SM.getFileID(Loc);
    if (!Scanned.insert(Id).second) return;
    llvm::StringRef Buffer = SM.getBufferData(Id);
    for (size_t I = Buffer.find("??"); I != llvm::StringRef::npos; I = Buffer.find("??", I + 1))
      if (I + 2 < Buffer.size() && llvm::StringRef("=(/)'<!>-").contains(Buffer[I + 2]))
        R.report(SM.getLocForStartOfFile(Id).getLocWithOffset(I), "trigraph sequence used");
  }

private:
  Rule &R;
  const SourceManager &SM;
  std::set<FileID> Scanned;
};

using CommentHandlers = std::vector<std::unique_ptr<CommentHandler>>;
Spec commentSpec(std::function<void(Rule &, SourceRange, llvm::StringRef)> F) {
  auto Handlers = std::make_shared<CommentHandlers>();
  Spec S;
  S.Preprocessor = [Handlers, F](Rule &R, Preprocessor &PP, const SourceManager &) {
    Handlers->push_back(onComments(PP, [&R, F](SourceRange Range, llvm::StringRef Text) { F(R, Range, Text); }));
  };
  return S;
}
} // namespace

void addRulesBasic(Table &T) {
  T.push_back({"misra-c2012-rule-1-1", rule11});
  T.push_back({"misra-c2012-rule-1-2", [] {
                 return unitSpec([](Rule &R, ASTContext &C) { ExtensionVisitor(R, C).TraverseDecl(C.getTranslationUnitDecl()); });
               }});
  T.push_back({"misra-c2012-rule-2-1", [] {
                 return matcherSpec(compoundStmt().bind("block"), [](Rule &R, const Result &Res) {
                   unreachable(R, *Res.Context, Res.Nodes.getNodeAs<CompoundStmt>("block"));
                 });
               }});
  T.push_back({"misra-c2012-rule-2-2", [] {
                 return unitSpec([](Rule &R, ASTContext &C) { DeadCode(R, C).TraverseDecl(C.getTranslationUnitDecl()); });
               }});
  T.push_back({"misra-c2012-rule-2-3", [] {
                 return matcherSpec(typedefNameDecl().bind("t"), [](Rule &R, const Result &Res) {
                   const auto *D = Res.Nodes.getNodeAs<TypedefNameDecl>("t");
                   if (isUserDecl(D) && Res.SourceManager->isInMainFile(D->getLocation()) && !D->isReferenced())
                     R.report(D->getLocation(), "type declaration is never used");
                 });
               }});
  T.push_back({"misra-c2012-rule-2-4", [] {
                 return unitSpec([](Rule &R, ASTContext &C) {
                   TagUses V(C.getSourceManager());
                   V.TraverseDecl(C.getTranslationUnitDecl());
                   std::set<const Decl *> Done;
                   auto Check = [&](const TagDecl *D) {
                     if (!D->getIdentifier() || !isUserDecl(D) || !C.getSourceManager().isInMainFile(D->getLocation()))
                       return;
                     if (!V.Used.count(D->getCanonicalDecl()) && Done.insert(D->getCanonicalDecl()).second)
                       R.report(D->getLocation(), "tag declaration is never used");
                   };
                   struct All : RecursiveASTVisitor<All> {
                     std::function<void(const TagDecl *)> F;
                     bool VisitTagDecl(TagDecl *D) { return F(D), true; }
                   } Walk;
                   Walk.F = Check;
                   Walk.TraverseDecl(C.getTranslationUnitDecl());
                 });
               }});
  T.push_back({"misra-c2012-rule-2-5", [] {
                 Spec S;
                 S.Preprocessor = [](Rule &R, Preprocessor &PP, const SourceManager &SM) {
                   PP.addPPCallbacks(std::make_unique<MacroUses>(R, SM));
                 };
                 return S;
               }});
  T.push_back({"misra-c2012-rule-2-6", [] {
                 return matcherSpec(functionDecl(isDefinition()).bind("f"), [](Rule &R, const Result &Res) {
                   const auto *F = Res.Nodes.getNodeAs<FunctionDecl>("f");
                   if (!isUserDecl(F)) return;
                   LabelUses V;
                   V.TraverseStmt(F->getBody());
                   for (const LabelDecl *L : V.Labels)
                     if (!V.Used.count(L)) R.report(L->getLocation(), "label is declared but never referenced");
                 });
               }});
  T.push_back({"misra-c2012-rule-2-7", [] {
                 return matcherSpec(functionDecl(isDefinition()).bind("f"), [](Rule &R, const Result &Res) {
                   const auto *F = Res.Nodes.getNodeAs<FunctionDecl>("f");
                   if (!isUserDecl(F)) return;
                   for (const ParmVarDecl *P : F->parameters())
                     if (!P->isReferenced() && !P->isUsed())
                       R.report(P->getLocation(), "function parameter is never used");
                 });
               }});
  T.push_back({"misra-c2012-rule-3-1", [] {
                 return commentSpec([](Rule &R, SourceRange Range, llvm::StringRef Text) {
                   bool Line = Text.starts_with("//");
                   llvm::StringRef Body = Text.drop_front(2);
                   if (!Line) Body.consume_back("*/");
                   if (Body.contains("/*") || (!Line && Body.contains("//")))
                     R.report(Range.getBegin(), "comment contains a comment start sequence");
                 });
               }});
  T.push_back({"misra-c2012-rule-3-2", [] {
                 return commentSpec([](Rule &R, SourceRange Range, llvm::StringRef Text) {
                   if (Text.starts_with("//") && (Text.contains("\\\n") || Text.contains("\\\r\n")))
                     R.report(Range.getBegin(), "line splice extends a // comment onto the next line");
                 });
               }});
  T.push_back({"misra-c2012-rule-4-1", [] {
                 Spec S;
                 S.Matchers = [](MatchFinder &F, Rule &R) {
                   F.addMatcher(stringLiteral().bind("s"), &R);
                   F.addMatcher(characterLiteral().bind("c"), &R);
                 };
                 S.Check = [](Rule &R, const Result &Res) {
                   const SourceManager &SM = *Res.SourceManager;
                   const LangOptions &LO = Res.Context->getLangOpts();
                   std::vector<SourceLocation> Tokens;
                   if (const auto *Str = Res.Nodes.getNodeAs<StringLiteral>("s"))
                     for (unsigned I = 0; I < Str->getNumConcatenated(); ++I) Tokens.push_back(Str->getStrTokenLoc(I));
                   if (const auto *Ch = Res.Nodes.getNodeAs<CharacterLiteral>("c")) Tokens.push_back(Ch->getLocation());
                   for (SourceLocation Tok : Tokens) {
                     if (!isUserLocation(SM, Tok)) continue;
                     if (auto Offset = unterminatedEscape(spelling(SM, LO, Tok)))
                       R.report(SM.getSpellingLoc(Tok).getLocWithOffset(*Offset),
                                "octal or hexadecimal escape sequence is not terminated");
                   }
                 };
                 return S;
               }});
  T.push_back({"misra-c2012-rule-4-2", [] {
                 Spec S;
                 S.Preprocessor = [](Rule &R, Preprocessor &PP, const SourceManager &SM) {
                   PP.addPPCallbacks(std::make_unique<Trigraphs>(R, SM));
                 };
                 return S;
               }});
}
} // namespace safety::misra
