// SPDX-License-Identifier: Apache-2.0
// Rules 5.x-9.x: identifiers, bit-fields, literals, declarations and
// initialization. System-scope rules are checked within one translation unit.
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
// ---- identifier inventory ------------------------------------------------
enum class Space { Ordinary, Tag, Member, Label };
// A scope is a block-like statement or a declaration (unit, function, record).
struct Scope {
  const Decl *D = nullptr;
  const Stmt *S = nullptr;
  bool operator==(const Scope &O) const { return D == O.D && S == O.S; }
  bool operator!=(const Scope &O) const { return !(*this == O); }
};
struct Ident {
  const NamedDecl *D;
  Space NS;
  Scope Where;
};
class Identifiers : public RecursiveASTVisitor<Identifiers> {
public:
  explicit Identifiers(ASTContext &C) : C(C) { TraverseDecl(C.getTranslationUnitDecl()); }
  Scope unit() const { return {C.getTranslationUnitDecl(), nullptr}; }
  bool VisitNamedDecl(NamedDecl *D) {
    if (!D->getIdentifier() || !isUserDecl(D)) return true;
    if (!isa<VarDecl, FunctionDecl, TypedefNameDecl, EnumConstantDecl, TagDecl, FieldDecl, LabelDecl>(D)) return true;
    if (const auto *P = dyn_cast<ParmVarDecl>(D)) {
      const auto *F = dyn_cast_or_null<FunctionDecl>(P->getDeclContext());
      if (!F || !F->doesThisDeclarationHaveABody()) return true;
    }
    Space NS = isa<TagDecl>(D) ? Space::Tag : isa<FieldDecl>(D) ? Space::Member : isa<LabelDecl>(D) ? Space::Label : Space::Ordinary;
    All.push_back({D, NS, scopeOf(D)});
    return true;
  }
  Scope scopeOf(const Decl *D) {
    if (isa<ParmVarDecl, FieldDecl, LabelDecl>(D)) return {cast<Decl>(D->getDeclContext()), nullptr};
    if (isa<EnumConstantDecl>(D)) return scopeOf(cast<Decl>(D->getDeclContext()));
    const DeclContext *DC = D->getDeclContext();
    if (isa<RecordDecl>(DC)) return scopeOf(cast<Decl>(DC));
    if (DC->isFileContext()) return unit();
    return enclosing(C.getParents(*D));
  }
  Scope enclosing(DynTypedNodeList Parents) {
    while (!Parents.empty()) {
      if (const auto *S = Parents[0].get<Stmt>()) {
        if (isa<CompoundStmt, ForStmt>(S)) return {nullptr, S};
        Parents = C.getParents(*S);
      } else if (const auto *D = Parents[0].get<Decl>()) {
        if (isa<FunctionDecl>(D)) return {D, nullptr};
        Parents = C.getParents(*D);
      } else
        break;
    }
    return unit();
  }
  // Enclosing scopes, innermost first, ending with the unit.
  std::vector<Scope> outer(Scope From) {
    std::vector<Scope> Out;
    while (From != unit()) {
      if (From.D && isa<RecordDecl>(From.D)) return {};
      From = From.S ? enclosing(C.getParents(*From.S)) : unit();
      Out.push_back(From);
    }
    return Out;
  }
  ASTContext &C;
  std::vector<Ident> All;
};
unsigned significant(const LangOptions &LO) { return LO.C99 ? 63 : 31; }
bool sameEntity(const NamedDecl *A, const NamedDecl *B) { return A->getCanonicalDecl() == B->getCanonicalDecl(); }
bool externalLinkage(const NamedDecl *D) {
  return isa<VarDecl, FunctionDecl>(D) && D->getFormalLinkage() == Linkage::External;
}
bool internalLinkage(const NamedDecl *D) {
  return isa<VarDecl, FunctionDecl>(D) && D->getFormalLinkage() == Linkage::Internal;
}
const NamedDecl *later(const SourceManager &SM, const NamedDecl *A, const NamedDecl *B) {
  return SM.isBeforeInTranslationUnit(A->getLocation(), B->getLocation()) ? B : A;
}
// Different names that agree in their first N characters.
bool indistinct(llvm::StringRef A, llvm::StringRef B, unsigned N) {
  return A != B && A.size() >= N && B.size() >= N && A.take_front(N) == B.take_front(N);
}

Spec rule51() {
  return unitSpec([](Rule &R, ASTContext &C) {
    Identifiers I(C);
    std::map<std::string, const NamedDecl *> ByPrefix;
    for (const Ident &X : I.All) {
      if (!externalLinkage(X.D)) continue;
      llvm::StringRef Name = X.D->getName();
      if (Name.size() < 31) continue;
      auto [It, Inserted] = ByPrefix.emplace(Name.take_front(31).str(), X.D);
      if (!Inserted && It->second->getName() != Name)
        R.report(X.D->getLocation(), ("external identifier is not distinct from '" + It->second->getName() +
                                      "' in its first 31 characters").str());
    }
  });
}
Spec rule52() {
  return unitSpec([](Rule &R, ASTContext &C) {
    Identifiers I(C);
    unsigned N = significant(C.getLangOpts());
    for (size_t A = 0; A < I.All.size(); ++A)
      for (size_t B = A + 1; B < I.All.size(); ++B) {
        const Ident &X = I.All[A], &Y = I.All[B];
        if (X.Where != Y.Where || X.NS != Y.NS || (externalLinkage(X.D) && externalLinkage(Y.D))) continue;
        if (indistinct(X.D->getName(), Y.D->getName(), N))
          R.report(later(C.getSourceManager(), X.D, Y.D)->getLocation(),
                   "identifier is not distinct from another in the same scope within the significant characters");
      }
  });
}
Spec rule53() {
  return unitSpec([](Rule &R, ASTContext &C) {
    Identifiers I(C);
    unsigned N = significant(C.getLangOpts());
    const SourceManager &SM = C.getSourceManager();
    for (const Ident &Inner : I.All) {
      if (Inner.Where == I.unit() || Inner.NS == Space::Member || Inner.NS == Space::Label) continue;
      for (Scope Enclosing : I.outer(Inner.Where))
        for (const Ident &Outer : I.All) {
          if (Outer.Where != Enclosing || Outer.NS != Inner.NS || sameEntity(Outer.D, Inner.D)) continue;
          llvm::StringRef A = Inner.D->getName(), B = Outer.D->getName();
          if ((A == B || indistinct(A, B, N)) && SM.isBeforeInTranslationUnit(Outer.D->getLocation(), Inner.D->getLocation()))
            R.report(Inner.D->getLocation(), ("declaration hides '" + B + "' from an outer scope").str());
        }
    }
  });
}
// Rules 5.6-5.9: a name must not be reused for a different entity.
bool associatedTag(const NamedDecl *A, const NamedDecl *B) {
  const auto *T = dyn_cast<TypedefNameDecl>(A);
  if (!T) std::swap(A, B), T = dyn_cast<TypedefNameDecl>(A);
  const auto *Tag = dyn_cast<TagDecl>(B);
  if (!T || !Tag) return false;
  const TagDecl *Under = T->getUnderlyingType()->getAsTagDecl();
  return Under && Under->getCanonicalDecl() == Tag->getCanonicalDecl();
}
Spec uniqueRule(std::function<bool(const NamedDecl *)> Subject, const char *What) {
  return unitSpec([Subject, What](Rule &R, ASTContext &C) {
    Identifiers I(C);
    const SourceManager &SM = C.getSourceManager();
    for (const Ident &X : I.All) {
      if (!Subject(X.D)) continue;
      for (const Ident &Y : I.All) {
        if (X.D == Y.D || X.D->getName() != Y.D->getName() || sameEntity(X.D, Y.D) || associatedTag(X.D, Y.D)) continue;
        if (Subject(Y.D) && !SM.isBeforeInTranslationUnit(X.D->getLocation(), Y.D->getLocation())) continue;
        R.report(Y.D->getLocation(), (llvm::Twine("name '") + Y.D->getName() + "' reuses " + What).str());
      }
    }
  });
}

// ---- Rules 5.4 / 5.5 -------------------------------------------------------
class MacroNames final : public PPCallbacks {
public:
  MacroNames(Rule &R, Preprocessor &PP, bool Distinct, std::shared_ptr<std::set<std::string>> All)
      : R(R), PP(PP), Distinct(Distinct), All(std::move(All)) {}
  void MacroDefined(const Token &Name, const MacroDirective *MD) override {
    llvm::StringRef Id = Name.getIdentifierInfo()->getName();
    unsigned N = significant(PP.getLangOpts());
    All->insert(Id.take_front(N).str());
    if (!Distinct) return;
    bool User = isUserLocation(PP.getSourceManager(), Name.getLocation());
    if (Id.size() >= N) {
      auto [It, Inserted] = Prefixes.emplace(Id.take_front(N).str(), Id.str());
      if (!Inserted && It->second != Id && User && PP.isMacroDefined(It->second))
        R.report(Name.getLocation(), "macro name is not distinct from '" + It->second + "'");
      It->second = Id.str();
    }
    if (!User) return;
    const MacroInfo *MI = MD->getMacroInfo();
    auto Params = MI->params();
    for (size_t A = 0; A < Params.size(); ++A) {
      llvm::StringRef P = Params[A]->getName();
      if (P.take_front(N) == Id.take_front(N))
        R.report(Name.getLocation(), "macro parameter is not distinct from the macro name");
      for (size_t B = A + 1; B < Params.size(); ++B)
        if (indistinct(P, Params[B]->getName(), N))
          R.report(Name.getLocation(), "macro parameters are not distinct from each other");
    }
  }

private:
  Rule &R;
  Preprocessor &PP;
  bool Distinct;
  std::shared_ptr<std::set<std::string>> All;
  std::map<std::string, std::string> Prefixes;
};

// ---- Rule 6.1 -------------------------------------------------------------
bool explicitlySigned(TypeLoc TL, ASTContext &C) {
  while (!TL.isNull()) {
    if (auto Q = TL.getAs<QualifiedTypeLoc>()) TL = Q.getUnqualifiedLoc();
    else if (auto E = TL.getAs<ElaboratedTypeLoc>()) TL = E.getNamedTypeLoc();
    else if (auto T = TL.getAs<TypedefTypeLoc>()) {
      const TypeSourceInfo *TSI = T.getTypedefNameDecl()->getTypeSourceInfo();
      if (!TSI) return false;
      TL = TSI->getTypeLoc();
    } else
      break;
  }
  std::string Text = sourceText(C.getSourceManager(), C.getLangOpts(), TL.getSourceRange());
  return Text.find("signed") != std::string::npos;
}

// ---- Rule 7.x literals ------------------------------------------------------
std::string literalText(const Result &Res, SourceLocation L) {
  return spelling(*Res.SourceManager, Res.Context->getLangOpts(), L);
}
llvm::StringRef suffixOf(llvm::StringRef Text) {
  size_t I = Text.size();
  while (I > 0 && llvm::isAlpha(Text[I - 1])) --I;
  return Text.substr(I);
}

// ---- Rule 7.4 -------------------------------------------------------------
void stringLiteralTarget(Rule &R, ASTContext &C, const Expr *Decay) {
  const Stmt *Child = Decay;
  QualType Target = Decay->getType();
  const Stmt *P = parentStmt(C, Child);
  while (const auto *Cast = dyn_cast_or_null<ImplicitCastExpr>(P)) {
    Target = Cast->getType();
    Child = P;
    P = parentStmt(C, P);
  }
  while (isa_and_nonnull<ParenExpr>(P)) {
    Child = P;
    P = parentStmt(C, P);
  }
  const auto *Ptr = Target->getAs<PointerType>();
  if (!Ptr || Ptr->getPointeeType().isConstQualified()) return;
  bool Assigned = false;
  if (!P) Assigned = isa_and_nonnull<VarDecl>(parentDecl(C, Child));
  else if (const auto *B = dyn_cast<BinaryOperator>(P)) Assigned = B->isAssignmentOp() && B->getRHS() == Child;
  else Assigned = isa<CallExpr, ReturnStmt, InitListExpr>(P);
  if (const auto *Sub = dyn_cast_or_null<ArraySubscriptExpr>(P)) {
    const Stmt *Up = parentStmt(C, Sub);
    while (isa_and_nonnull<ParenExpr, ImplicitCastExpr>(Up)) Up = parentStmt(C, Up);
    if (const auto *B = dyn_cast_or_null<BinaryOperator>(Up); B && B->isAssignmentOp() && B->getLHS()->IgnoreParens() == Sub) {
      R.report(Sub->getBeginLoc(), "string literal is modified");
      return;
    }
  }
  if (Assigned) R.report(Decay->getBeginLoc(), "string literal is assigned to a pointer to non-const characters");
}

// ---- Rule 8.x ---------------------------------------------------------------
std::string writtenType(const DeclaratorDecl *D) {
  return D->getType().getAsString();
}

// ---- Rule 9.x initializers --------------------------------------------------
bool zeroList(const InitListExpr *L) {
  if (!L) return false;
  const InitListExpr *Syntax = L->isSemanticForm() && L->getSyntacticForm() ? L->getSyntacticForm() : L;
  if (Syntax->getNumInits() != 1) return false;
  const auto *I = dyn_cast<IntegerLiteral>(stripAll(Syntax->getInit(0)));
  return I && I->getValue() == 0;
}
// Brace elision leaves sub-lists whose "brace" locations point at initializers.
bool braced(const SourceManager &SM, const InitListExpr *L) {
  SourceLocation B = L->getLBraceLoc();
  return B.isValid() && *SM.getCharacterData(SM.getSpellingLoc(B)) == '{';
}
void designatedLeaves(const Stmt *S, std::set<const Expr *> &Out) {
  if (!S) return;
  if (const auto *D = dyn_cast<DesignatedInitExpr>(S)) Out.insert(D->getInit()->IgnoreImplicit());
  for (const Stmt *Child : S->children()) designatedLeaves(Child, Out);
}
// Classify the explicit leaf initializers of a semantic list.
void leaves(const SourceManager &SM, const InitListExpr *L, const std::set<const Expr *> &Designated,
            bool &AnyPositional, bool &AnyDesignated) {
  for (const Expr *Init : L->inits()) {
    if (!Init || isa<ImplicitValueInitExpr>(Init)) continue;
    if (const auto *Sub = dyn_cast<InitListExpr>(Init)) {
      if (braced(SM, Sub)) {
        AnyPositional = true;
        continue;
      }
      leaves(SM, Sub, Designated, AnyPositional, AnyDesignated);
      continue;
    }
    (Designated.count(Init->IgnoreImplicit()) ? AnyDesignated : AnyPositional) = true;
  }
}
const InitListExpr *outermost(ASTContext &C, const InitListExpr *L) {
  const InitListExpr *Top = L;
  for (const Stmt *S = parentStmt(C, L); isa_and_nonnull<InitListExpr>(S); S = parentStmt(C, S)) Top = cast<InitListExpr>(S);
  return Top;
}
bool insideZeroList(ASTContext &C, const InitListExpr *L) {
  for (const Stmt *S = L; isa_and_nonnull<InitListExpr>(S); S = parentStmt(C, S))
    if (zeroList(cast<InitListExpr>(S))) return true;
  return false;
}
std::set<const Expr *> designatedIn(const InitListExpr *Top) {
  std::set<const Expr *> Out;
  designatedLeaves(Top->isSemanticForm() && Top->getSyntacticForm() ? Top->getSyntacticForm() : Top, Out);
  return Out;
}
void initRule92(Rule &R, ASTContext &C, const InitListExpr *L) {
  const SourceManager &SM = C.getSourceManager();
  if (!L->isSemanticForm() || !braced(SM, L) || insideZeroList(C, L)) return;
  auto Designated = designatedIn(outermost(C, L));
  for (const Expr *Init : L->inits()) {
    const auto *Sub = dyn_cast_or_null<InitListExpr>(Init);
    if (!Sub || braced(SM, Sub)) continue;
    bool Positional = false, Desig = false;
    leaves(SM, Sub, Designated, Positional, Desig);
    if (Positional) R.report(Sub->getBeginLoc(), "subobject initializer is not enclosed in braces");
  }
}
void initRule93(Rule &R, ASTContext &C, const InitListExpr *L) {
  if (!L->isSemanticForm() || insideZeroList(C, L)) return;
  const auto *Array = C.getAsConstantArrayType(L->getType());
  if (!Array) return;
  bool Positional = false, Desig = false;
  leaves(C.getSourceManager(), L, designatedIn(outermost(C, L)), Positional, Desig);
  if (!Positional && Desig) return;
  if (!Positional && !Desig) return;
  uint64_t Explicit = 0;
  for (const Expr *Init : L->inits())
    if (Init && !isa<ImplicitValueInitExpr>(Init)) ++Explicit;
  if (Explicit < Array->getSize().getZExtValue())
    R.report(L->getBeginLoc().isValid() ? L->getBeginLoc() : L->getExprLoc(), "array is only partially initialized");
}
// Designator paths such as "[1].a" for the top level of a syntactic list.
void initRule94(Rule &R, ASTContext &C, const InitListExpr *L) {
  if (L->isSemanticForm() && L->getSyntacticForm()) L = L->getSyntacticForm();
  std::map<std::string, SourceLocation> Seen;
  int64_t Next = 0;
  for (const Expr *Init : L->inits()) {
    std::string Path;
    if (const auto *D = dyn_cast<DesignatedInitExpr>(Init)) {
      bool First = true;
      for (const auto &Des : D->designators()) {
        if (Des.isFieldDesignator()) {
          const FieldDecl *F = Des.getFieldDecl();
          Path += "." + (F ? std::to_string(F->getFieldIndex()) : Des.getFieldName()->getName().str());
          if (First && F) Next = F->getFieldIndex() + 1;
        } else if (Des.isArrayDesignator()) {
          auto V = D->getArrayIndex(Des)->getIntegerConstantExpr(C);
          if (!V) return;
          Path += "[" + std::to_string(V->getExtValue()) + "]";
          if (First) Next = V->getExtValue() + 1;
        } else
          return;
        First = false;
      }
    } else {
      Path = "[" + std::to_string(Next) + "]";
      if (L->getType()->isRecordType()) Path = "." + std::to_string(Next);
      ++Next;
    }
    for (const auto &[Other, Loc] : Seen)
      if (llvm::StringRef(Path).starts_with(Other) || llvm::StringRef(Other).starts_with(Path)) {
        R.report(Init->getBeginLoc(), "element is initialized more than once");
        break;
      }
    Seen.emplace(Path, Init->getBeginLoc());
  }
}
} // namespace

void addRulesDeclarations(Table &T) {
  T.push_back({"misra-c2012-rule-5-1", rule51});
  T.push_back({"misra-c2012-rule-5-2", rule52});
  T.push_back({"misra-c2012-rule-5-3", rule53});
  T.push_back({"misra-c2012-rule-5-4", [] {
                 Spec S;
                 S.Preprocessor = [](Rule &R, Preprocessor &PP, const SourceManager &) {
                   PP.addPPCallbacks(std::make_unique<MacroNames>(R, PP, true, std::make_shared<std::set<std::string>>()));
                 };
                 return S;
               }});
  T.push_back({"misra-c2012-rule-5-5", [] {
                 auto Names = std::make_shared<std::set<std::string>>();
                 Spec S = unitSpec([Names](Rule &R, ASTContext &C) {
                   Identifiers I(C);
                   unsigned N = significant(C.getLangOpts());
                   for (const Ident &X : I.All)
                     if (Names->count(X.D->getName().take_front(N).str()))
                       R.report(X.D->getLocation(), "identifier is not distinct from a macro name");
                 });
                 S.Preprocessor = [Names](Rule &R, Preprocessor &PP, const SourceManager &) {
                   PP.addPPCallbacks(std::make_unique<MacroNames>(R, PP, false, Names));
                 };
                 return S;
               }});
  T.push_back({"misra-c2012-rule-5-6", [] {
                 return uniqueRule([](const NamedDecl *D) { return isa<TypedefNameDecl>(D); }, "a typedef name");
               }});
  T.push_back({"misra-c2012-rule-5-7", [] {
                 return uniqueRule([](const NamedDecl *D) { return isa<TagDecl>(D); }, "a tag name");
               }});
  T.push_back({"misra-c2012-rule-5-8", [] {
                 return uniqueRule(externalLinkage, "an identifier with external linkage");
               }});
  T.push_back({"misra-c2012-rule-5-9", [] {
                 return uniqueRule(internalLinkage, "an identifier with internal linkage");
               }});
  T.push_back({"misra-c2012-rule-6-1", [] {
                 return matcherSpec(fieldDecl(isBitField()).bind("f"), [](Rule &R, const Result &Res) {
                   const auto *F = Res.Nodes.getNodeAs<FieldDecl>("f");
                   if (!isUserDecl(F)) return;
                   QualType T = F->getType().getCanonicalType();
                   bool Ok = T->isSpecificBuiltinType(BuiltinType::UInt) ||
                             (T->isBooleanType() && Res.Context->getLangOpts().C99) ||
                             (T->isSpecificBuiltinType(BuiltinType::Int) &&
                              explicitlySigned(F->getTypeSourceInfo()->getTypeLoc(), *Res.Context));
                   if (!Ok) R.report(F->getLocation(), "bit-field type is not explicitly signed int, unsigned int or _Bool");
                 });
               }});
  T.push_back({"misra-c2012-rule-6-2", [] {
                 return matcherSpec(fieldDecl(isBitField()).bind("f"), [](Rule &R, const Result &Res) {
                   const auto *F = Res.Nodes.getNodeAs<FieldDecl>("f");
                   if (isUserDecl(F) && F->getIdentifier() && F->getBitWidthValue(*Res.Context) == 1 &&
                       F->getType()->isSignedIntegerType())
                     R.report(F->getLocation(), "named single-bit bit-field has a signed type");
                 });
               }});
  T.push_back({"misra-c2012-rule-7-1", [] {
                 return matcherSpec(integerLiteral().bind("i"), [](Rule &R, const Result &Res) {
                   const auto *I = Res.Nodes.getNodeAs<IntegerLiteral>("i");
                   if (!isUserLocation(*Res.SourceManager, I->getLocation())) return;
                   std::string Text = literalText(Res, I->getLocation());
                   if (Text.size() > 1 && Text[0] == '0' && llvm::isDigit(Text[1]))
                     R.report(I->getLocation(), "octal constant used");
                 });
               }});
  T.push_back({"misra-c2012-rule-7-2", [] {
                 return matcherSpec(integerLiteral().bind("i"), [](Rule &R, const Result &Res) {
                   const auto *I = Res.Nodes.getNodeAs<IntegerLiteral>("i");
                   if (!isUserLocation(*Res.SourceManager, I->getLocation()) || !I->getType()->isUnsignedIntegerType()) return;
                   std::string Text = literalText(Res, I->getLocation());
                   if (!Text.empty() && suffixOf(Text).find_insensitive("u") == llvm::StringRef::npos)
                     R.report(I->getLocation(), "unsigned integer constant has no 'U' suffix");
                 });
               }});
  T.push_back({"misra-c2012-rule-7-3", [] {
                 Spec S;
                 S.Matchers = [](MatchFinder &F, Rule &R) {
                   F.addMatcher(integerLiteral().bind("l"), &R);
                   F.addMatcher(floatLiteral().bind("l"), &R);
                 };
                 S.Check = [](Rule &R, const Result &Res) {
                   const auto *E = Res.Nodes.getNodeAs<Expr>("l");
                   if (!isUserLocation(*Res.SourceManager, E->getExprLoc())) return;
                   if (suffixOf(literalText(Res, E->getExprLoc())).contains('l'))
                     R.report(E->getExprLoc(), "lowercase 'l' used in a literal suffix");
                 };
                 return S;
               }});
  T.push_back({"misra-c2012-rule-7-4", [] {
                 return matcherSpec(implicitCastExpr(hasCastKind(CK_ArrayToPointerDecay),
                                                     hasSourceExpression(ignoringParens(stringLiteral())))
                                        .bind("decay"),
                                    [](Rule &R, const Result &Res) {
                                      const auto *E = Res.Nodes.getNodeAs<Expr>("decay");
                                      if (isUserLocation(*Res.SourceManager, E->getBeginLoc()))
                                        stringLiteralTarget(R, *Res.Context, E);
                                    });
               }});
  T.push_back({"misra-c2012-rule-8-1", [] {
                 return matcherSpec(declaratorDecl().bind("d"), [](Rule &R, const Result &Res) {
                   const auto *D = Res.Nodes.getNodeAs<DeclaratorDecl>("d");
                   if (!isUserDecl(D) || !D->getTypeSourceInfo()) return;
                   TypeLoc TL = D->getTypeSourceInfo()->getTypeLoc();
                   if (auto F = TL.getAs<FunctionTypeLoc>()) TL = F.getReturnLoc();
                   while (auto Q = TL.getAs<QualifiedTypeLoc>()) TL = Q.getUnqualifiedLoc();
                   auto B = TL.getAs<BuiltinTypeLoc>();
                   if (!B || !B.getTypePtr()->isSpecificBuiltinType(BuiltinType::Int)) return;
                   std::string Text = sourceText(*Res.SourceManager, Res.Context->getLangOpts(), B.getSourceRange());
                   if (Text.find("int") == std::string::npos && Text.find("signed") == std::string::npos)
                     R.report(D->getLocation(), "type is not explicitly specified (implicit int)");
                 });
               }});
  T.push_back({"misra-c2012-rule-8-2", [] {
                 return unitSpec([](Rule &R, ASTContext &C) {
                   struct V : RecursiveASTVisitor<V> {
                     Rule &R;
                     ASTContext &C;
                     V(Rule &R, ASTContext &C) : R(R), C(C) {}
                     bool user(SourceLocation L) { return isUserLocation(C.getSourceManager(), L); }
                     bool VisitFunctionNoProtoTypeLoc(FunctionNoProtoTypeLoc TL) {
                       if (user(TL.getBeginLoc())) R.report(TL.getBeginLoc(), "function type is not in prototype form");
                       return true;
                     }
                     bool VisitFunctionProtoTypeLoc(FunctionProtoTypeLoc TL) {
                       for (const ParmVarDecl *P : TL.getParams())
                         if (P && !P->getIdentifier() && user(P->getLocation()))
                           R.report(P->getLocation().isValid() ? P->getLocation() : TL.getBeginLoc(),
                                    "function parameter is not named");
                       return true;
                     }
                     bool VisitFunctionDecl(FunctionDecl *F) {
                       if (isUserDecl(F) && !F->hasWrittenPrototype() && F->getNumParams() > 0)
                         R.report(F->getLocation(), "function uses an old-style parameter declaration list");
                       return true;
                     }
                   } Walk(R, C);
                   Walk.TraverseDecl(C.getTranslationUnitDecl());
                 });
               }});
  T.push_back({"misra-c2012-rule-8-3", [] {
                 return matcherSpec(declaratorDecl(anyOf(functionDecl(), varDecl(unless(parmVarDecl())))).bind("d"),
                                    [](Rule &R, const Result &Res) {
                   const auto *D = Res.Nodes.getNodeAs<DeclaratorDecl>("d");
                   const auto *Prev = dyn_cast_or_null<DeclaratorDecl>(D->getPreviousDecl());
                   if (!isUserDecl(D) || !Prev || !isUserDecl(Prev)) return;
                   const auto *F = dyn_cast<FunctionDecl>(D), *PF = dyn_cast<FunctionDecl>(Prev);
                   if (!F) {
                     if (writtenType(D) != writtenType(Prev))
                       R.report(D->getLocation(), "declaration type differs from a previous declaration");
                     return;
                   }
                   if (F->getReturnType().getAsString() != PF->getReturnType().getAsString())
                     R.report(D->getLocation(), "return type differs from a previous declaration");
                   for (unsigned I = 0; I < F->getNumParams() && I < PF->getNumParams(); ++I) {
                     const ParmVarDecl *A = F->getParamDecl(I), *B = PF->getParamDecl(I);
                     if (A->getIdentifier() && B->getIdentifier() && A->getName() != B->getName())
                       R.report(A->getLocation(), "parameter name differs from a previous declaration");
                     else if (A->getOriginalType().getAsString() != B->getOriginalType().getAsString())
                       R.report(A->getLocation(), "parameter type or qualifiers differ from a previous declaration");
                   }
                 });
               }});
  T.push_back({"misra-c2012-rule-8-4", [] {
                 return matcherSpec(declaratorDecl(anyOf(functionDecl(isDefinition()), varDecl(hasGlobalStorage()))).bind("d"),
                                    [](Rule &R, const Result &Res) {
                   const auto *D = Res.Nodes.getNodeAs<DeclaratorDecl>("d");
                   if (!isUserDecl(D) || D->getFormalLinkage() != Linkage::External || D->getPreviousDecl()) return;
                   if (const auto *F = dyn_cast<FunctionDecl>(D); F && F->isMain()) return;
                   if (const auto *V = dyn_cast<VarDecl>(D);
                       V && (!V->isFileVarDecl() || V->isThisDeclarationADefinition() == VarDecl::DeclarationOnly))
                     return;
                   R.report(D->getLocation(), "external definition has no prior compatible declaration");
                 });
               }});
  T.push_back({"misra-c2012-rule-8-5", [] {
                 return matcherSpec(declaratorDecl(anyOf(functionDecl(), varDecl(hasGlobalStorage()))).bind("d"),
                                    [](Rule &R, const Result &Res) {
                   const auto *D = Res.Nodes.getNodeAs<DeclaratorDecl>("d");
                   if (!isUserDecl(D) || D->getFormalLinkage() != Linkage::External) return;
                   auto Defining = [](const Decl *X) {
                     if (const auto *F = dyn_cast<FunctionDecl>(X)) return F->doesThisDeclarationHaveABody();
                     return cast<VarDecl>(X)->isThisDeclarationADefinition() != VarDecl::DeclarationOnly;
                   };
                   if (Defining(D)) return;
                   for (const Decl *Prev = D->getPreviousDecl(); Prev; Prev = Prev->getPreviousDecl())
                     if (!Defining(Prev) && isUserDecl(Prev)) {
                       R.report(D->getLocation(), "external object or function is declared more than once");
                       return;
                     }
                 });
               }});
  T.push_back({"misra-c2012-rule-8-7", [] {
                 return matcherSpec(declaratorDecl(anyOf(functionDecl(isDefinition()), varDecl(hasGlobalStorage()))).bind("d"),
                                    [](Rule &R, const Result &Res) {
                   const auto *D = Res.Nodes.getNodeAs<DeclaratorDecl>("d");
                   const SourceManager &SM = *Res.SourceManager;
                   if (!isUserDecl(D) || D->getFormalLinkage() != Linkage::External || !SM.isInMainFile(D->getLocation()))
                     return;
                   if (const auto *F = dyn_cast<FunctionDecl>(D); F && F->isMain()) return;
                   if (const auto *V = dyn_cast<VarDecl>(D); V && (!V->isFileVarDecl() || V->isThisDeclarationADefinition() == VarDecl::DeclarationOnly))
                     return;
                   for (const Decl *Other : D->redecls())
                     if (!SM.isInMainFile(Other->getLocation())) return;
                   R.report(D->getLocation(), "external linkage is given to an entity that no header declares; "
                                              "it appears to be referenced only in this translation unit");
                 });
               }});
  T.push_back({"misra-c2012-rule-8-8", [] {
                 return matcherSpec(declaratorDecl(anyOf(functionDecl(), varDecl(hasGlobalStorage()))).bind("d"),
                                    [](Rule &R, const Result &Res) {
                   const auto *D = Res.Nodes.getNodeAs<DeclaratorDecl>("d");
                   if (!isUserDecl(D) || D->getFormalLinkage() != Linkage::Internal) return;
                   StorageClass SC = isa<FunctionDecl>(D) ? cast<FunctionDecl>(D)->getStorageClass() : cast<VarDecl>(D)->getStorageClass();
                   if (SC != SC_Static) R.report(D->getLocation(), "declaration of an internal-linkage entity omits 'static'");
                 });
               }});
  T.push_back({"misra-c2012-rule-8-9", [] {
                 return unitSpec([](Rule &R, ASTContext &C) {
                   struct V : RecursiveASTVisitor<V> {
                     const FunctionDecl *Current = nullptr;
                     std::map<const VarDecl *, std::set<const Decl *>> Users;
                     bool TraverseFunctionDecl(FunctionDecl *F) {
                       const FunctionDecl *Saved = Current;
                       Current = F;
                       bool Continue = RecursiveASTVisitor::TraverseFunctionDecl(F);
                       Current = Saved;
                       return Continue;
                     }
                     bool VisitDeclRefExpr(DeclRefExpr *E) {
                       if (const auto *Var = dyn_cast<VarDecl>(E->getDecl()); Var && Var->isFileVarDecl())
                         Users[Var->getCanonicalDecl()].insert(Current ? static_cast<const Decl *>(Current) : Var);
                       return true;
                     }
                   } Walk;
                   Walk.TraverseDecl(C.getTranslationUnitDecl());
                   for (const auto &[Var, Where] : Walk.Users) {
                     const VarDecl *Def = Var->getDefinition() ? Var->getDefinition() : Var;
                     if (isUserDecl(Def) && Var->getFormalLinkage() == Linkage::Internal && Where.size() == 1 &&
                         isa<FunctionDecl>(*Where.begin()))
                       R.report(Def->getLocation(), "object is only used in one function and could be defined at block scope");
                   }
                 });
               }});
  T.push_back({"misra-c2012-rule-8-10", [] {
                 return matcherSpec(functionDecl(isInline()).bind("f"), [](Rule &R, const Result &Res) {
                   const auto *F = Res.Nodes.getNodeAs<FunctionDecl>("f");
                   if (isUserDecl(F) && F->isInlineSpecified() && F->getStorageClass() != SC_Static)
                     R.report(F->getLocation(), "inline function is not declared static");
                 });
               }});
  T.push_back({"misra-c2012-rule-8-11", [] {
                 return matcherSpec(varDecl(hasType(incompleteArrayType())).bind("v"), [](Rule &R, const Result &Res) {
                   const auto *V = Res.Nodes.getNodeAs<VarDecl>("v");
                   if (isUserDecl(V) && V->getFormalLinkage() == Linkage::External &&
                       V->isThisDeclarationADefinition() == VarDecl::DeclarationOnly)
                     R.report(V->getLocation(), "external array is declared without an explicit size");
                 });
               }});
  T.push_back({"misra-c2012-rule-8-12", [] {
                 return matcherSpec(enumDecl(isDefinition()).bind("e"), [](Rule &R, const Result &Res) {
                   const auto *E = Res.Nodes.getNodeAs<EnumDecl>("e");
                   if (!isUserDecl(E)) return;
                   for (const EnumConstantDecl *A : E->enumerators()) {
                     if (A->getInitExpr()) continue;
                     for (const EnumConstantDecl *B : E->enumerators())
                       if (A != B && llvm::APSInt::isSameValue(A->getInitVal(), B->getInitVal())) {
                         R.report(A->getLocation(), "implicitly valued enumeration constant duplicates another value");
                         break;
                       }
                   }
                 });
               }});
  T.push_back({"misra-c2012-rule-8-14", [] {
                 return matcherSpec(decl(anyOf(declaratorDecl(), typedefNameDecl())).bind("d"), [](Rule &R, const Result &Res) {
                   const auto *D = Res.Nodes.getNodeAs<Decl>("d");
                   if (!isUserDecl(D)) return;
                   QualType T = isa<TypedefNameDecl>(D) ? cast<TypedefNameDecl>(D)->getUnderlyingType() : cast<DeclaratorDecl>(D)->getType();
                   std::function<bool(QualType)> Restrict = [&](QualType Q) -> bool {
                     if (Q.isNull()) return false;
                     if (Q.isRestrictQualified()) return true;
                     if (const auto *P = Q->getAs<PointerType>()) return Restrict(P->getPointeeType());
                     if (const auto *A = Q->getAsArrayTypeUnsafe()) return Restrict(A->getElementType());
                     if (const auto *F = Q->getAs<FunctionProtoType>()) {
                       for (QualType P : F->getParamTypes())
                         if (Restrict(P)) return true;
                       return Restrict(F->getReturnType());
                     }
                     return false;
                   };
                   if (isa<FunctionDecl>(D)) T = cast<FunctionDecl>(D)->getReturnType();
                   if (const auto *P = dyn_cast<ParmVarDecl>(D)) T = P->getOriginalType();
                   if (Restrict(T)) R.report(D->getLocation(), "restrict type qualifier used");
                 });
               }});
  T.push_back({"misra-c2012-rule-9-2", [] {
                 return matcherSpec(initListExpr().bind("l"), [](Rule &R, const Result &Res) {
                   const auto *L = Res.Nodes.getNodeAs<InitListExpr>("l");
                   if (isUserLocation(*Res.SourceManager, L->getBeginLoc())) initRule92(R, *Res.Context, L);
                 });
               }});
  T.push_back({"misra-c2012-rule-9-3", [] {
                 return matcherSpec(initListExpr().bind("l"), [](Rule &R, const Result &Res) {
                   const auto *L = Res.Nodes.getNodeAs<InitListExpr>("l");
                   if (isUserLocation(*Res.SourceManager, L->getExprLoc())) initRule93(R, *Res.Context, L);
                 });
               }});
  T.push_back({"misra-c2012-rule-9-4", [] {
                 return matcherSpec(initListExpr().bind("l"), [](Rule &R, const Result &Res) {
                   const auto *L = Res.Nodes.getNodeAs<InitListExpr>("l");
                   if (isUserLocation(*Res.SourceManager, L->getBeginLoc())) initRule94(R, *Res.Context, L);
                 });
               }});
  T.push_back({"misra-c2012-rule-9-5", [] {
                 return matcherSpec(varDecl(hasInitializer(initListExpr())).bind("v"), [](Rule &R, const Result &Res) {
                   const auto *V = Res.Nodes.getNodeAs<VarDecl>("v");
                   if (!isUserDecl(V) || !V->getTypeSourceInfo()) return;
                   if (!V->getTypeSourceInfo()->getTypeLoc().getAs<IncompleteArrayTypeLoc>()) return;
                   const auto *L = cast<InitListExpr>(V->getInit()->IgnoreImplicit());
                   if (const InitListExpr *Syntax = L->getSyntacticForm()) L = Syntax;
                   for (const Expr *Init : L->inits())
                     if (const auto *D = dyn_cast<DesignatedInitExpr>(Init); D && D->getDesignator(0)->isArrayDesignator()) {
                       R.report(V->getLocation(), "array size is implied by designated initializers");
                       return;
                     }
                 });
               }});
}
} // namespace safety::misra
