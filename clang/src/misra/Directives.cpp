// SPDX-License-Identifier: Apache-2.0
// Directive checks. Directives are only partly decidable; each check reports
// source evidence that a reviewer must assess against project documentation.
#include "Misra.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Lex/MacroArgs.h"
#include "clang/Lex/MacroInfo.h"
#include "llvm/ADT/StringSet.h"
#include <map>
#include <set>

using namespace clang;
using namespace clang::ast_matchers;

namespace safety::misra {
namespace {
// ---- Dir 1.1 -------------------------------------------------------------
class PragmaCallbacks final : public PPCallbacks {
public:
  PragmaCallbacks(Rule &R, const SourceManager &SM) : R(R), SM(SM) {}
  void PragmaDirective(SourceLocation Loc, PragmaIntroducerKind) override {
    if (isUserLocation(SM, Loc)) R.report(Loc, "#pragma behaviour is implementation-defined and must be documented");
  }

private:
  Rule &R;
  const SourceManager &SM;
};
Spec dir11() {
  Spec S = matcherSpec(binaryOperator(hasAnyOperatorName(">>", ">>=", "/", "%", "/=", "%=")).bind("op"),
                       [](Rule &R, const Result &Res) {
                         const auto *B = Res.Nodes.getNodeAs<BinaryOperator>("op");
                         ASTContext &C = *Res.Context;
                         if (!isUserLocation(*Res.SourceManager, B->getOperatorLoc())) return;
                         EType L = essentialType(B->getLHS(), C);
                         if (B->isShiftOp() || B->isShiftAssignOp()) {
                           auto V = integerConstant(B->getLHS(), C);
                           if (L.C == Cat::Signed && !(V && !V->isNegative()))
                             R.report(B->getOperatorLoc(), "right shift of a signed value has implementation-defined "
                                                           "results for negative values");
                           return;
                         }
                         if (C.getLangOpts().C99 || isConstantExpression(B, C)) return;
                         if (L.C == Cat::Signed && essentialType(B->getRHS(), C).C == Cat::Signed)
                           R.report(B->getOperatorLoc(), "signed division rounding is implementation-defined in C90");
                       });
  S.Preprocessor = [](Rule &R, Preprocessor &PP, const SourceManager &SM) {
    PP.addPPCallbacks(std::make_unique<PragmaCallbacks>(R, SM));
  };
  return S;
}

// ---- Dir 4.2 / 4.3 -------------------------------------------------------
Spec dir42() {
  Spec S;
  S.Matchers = [](MatchFinder &F, Rule &R) {
    F.addMatcher(asmStmt().bind("asm"), &R);
    F.addMatcher(decl(isExpansionInMainFile()).bind("file"), &R);
  };
  S.Check = [](Rule &R, const Result &Res) {
    SourceLocation L;
    if (const auto *A = Res.Nodes.getNodeAs<AsmStmt>("asm")) L = A->getAsmLoc();
    if (const auto *D = dyn_cast_or_null<FileScopeAsmDecl>(Res.Nodes.getNodeAs<Decl>("file"))) L = D->getAsmLoc();
    if (isUserLocation(*Res.SourceManager, L))
      R.report(L, "assembly language use must be documented with its rationale and interface");
  };
  return S;
}
Spec dir43() {
  return matcherSpec(asmStmt(hasAncestor(functionDecl().bind("fn"))).bind("asm"), [](Rule &R, const Result &Res) {
    const auto *A = Res.Nodes.getNodeAs<AsmStmt>("asm");
    const auto *F = Res.Nodes.getNodeAs<FunctionDecl>("fn");
    if (A->getAsmLoc().isMacroID() || !isUserLocation(*Res.SourceManager, A->getAsmLoc())) return;
    const auto *Body = dyn_cast_or_null<CompoundStmt>(F->getBody());
    if (!Body) return;
    for (const Stmt *S : Body->body()) {
      if (isa<AsmStmt, NullStmt>(S)) continue;
      if (const auto *Ret = dyn_cast<ReturnStmt>(S); Ret && !Ret->getRetValue()) continue;
      R.report(A->getAsmLoc(), "assembly language is mixed with C statements; isolate it in a dedicated function or macro");
      return;
    }
  });
}

// ---- Dir 4.4 -------------------------------------------------------------
bool looksLikeCode(llvm::StringRef Line) {
  Line = Line.trim();
  while (Line.consume_front("*")) Line = Line.ltrim();
  Line = Line.trim();
  if (Line.size() < 2) return false;
  if (Line.starts_with("#include") || Line.starts_with("#define") || Line.starts_with("#if") ||
      Line.starts_with("#ifdef") || Line.starts_with("#endif"))
    return true;
  char Last = Line.back();
  if (Last == ';' || Last == '{' || Last == '}') {
    // Prose sentences rarely end with these; require a code-like character.
    return Line.find_first_of("=();{}[]") != llvm::StringRef::npos;
  }
  return false;
}
Spec dir44() {
  auto Handlers = std::make_shared<std::vector<std::unique_ptr<CommentHandler>>>();
  Spec S;
  S.Preprocessor = [Handlers](Rule &R, Preprocessor &PP, const SourceManager &) {
    Handlers->push_back(onComments(PP, [&R](SourceRange Range, llvm::StringRef Text) {
      llvm::StringRef Body = Text;
      if (Body.starts_with("//")) Body = Body.drop_front(2);
      else {
        Body = Body.drop_front(2);
        Body.consume_back("*/");
      }
      llvm::SmallVector<llvm::StringRef, 16> Lines;
      Body.split(Lines, '\n');
      for (llvm::StringRef Line : Lines)
        if (looksLikeCode(Line)) {
          R.report(Range.getBegin(), "comment appears to contain commented-out code");
          return;
        }
    }));
  };
  return S;
}

// ---- Dir 4.5 -------------------------------------------------------------
std::string ambiguityKey(llvm::StringRef Name) {
  std::string Lower;
  for (char Ch : Name)
    if (Ch != '_') Lower.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(Ch))));
  std::string Merged;
  for (size_t I = 0; I < Lower.size(); ++I) {
    if (Lower[I] == 'r' && I + 1 < Lower.size() && Lower[I + 1] == 'n') {
      Merged.push_back('m');
      ++I;
    } else
      Merged.push_back(Lower[I]);
  }
  for (char &Ch : Merged) {
    switch (Ch) {
    case 'o': Ch = '0'; break;
    case 'i':
    case 'l': Ch = '1'; break;
    case 's': Ch = '5'; break;
    case 'z': Ch = '2'; break;
    case 'b': Ch = '8'; break;
    case 'h': Ch = 'n'; break;
    default: break;
    }
  }
  return Merged;
}
class Collector : public RecursiveASTVisitor<Collector> {
public:
  explicit Collector(ASTContext &C) : C(C) {}
  bool VisitNamedDecl(NamedDecl *D) {
    if (!isUserDecl(D) || !D->getIdentifier()) return true;
    if (!isa<VarDecl, FunctionDecl, TypedefNameDecl, EnumConstantDecl, TagDecl>(D)) return true;
    const DeclContext *Ctx = D->getDeclContext();
    if (isa<EnumConstantDecl>(D)) Ctx = Ctx->getParent();
    if (isa<RecordDecl>(Ctx)) return true;
    const auto *Fn = dyn_cast<FunctionDecl>(Ctx->getRedeclContext());
    if (const auto *P = dyn_cast<ParmVarDecl>(D)) Fn = dyn_cast<FunctionDecl>(P->getDeclContext());
    if (const auto *P = dyn_cast<ParmVarDecl>(D); P && !Fn) return true;
    (isa<TagDecl>(D) ? Tags : Ordinary)[Fn].push_back(D);
    return true;
  }
  ASTContext &C;
  std::map<const FunctionDecl *, std::vector<const NamedDecl *>> Ordinary, Tags;
};
void compareAmbiguous(Rule &R, const SourceManager &SM, const std::vector<const NamedDecl *> &A,
                      const std::vector<const NamedDecl *> &B, bool Same) {
  for (size_t I = 0; I < A.size(); ++I)
    for (size_t J = Same ? I + 1 : 0; J < B.size(); ++J) {
      const NamedDecl *X = A[I], *Y = B[J];
      if (X->getName() == Y->getName() || ambiguityKey(X->getName()) != ambiguityKey(Y->getName())) continue;
      const NamedDecl *Later = SM.isBeforeInTranslationUnit(X->getLocation(), Y->getLocation()) ? Y : X;
      const NamedDecl *Other = Later == X ? Y : X;
      R.report(Later->getLocation(), ("identifier '" + Later->getName() + "' is typographically ambiguous with '" +
                                      Other->getName() + "'").str());
    }
}
Spec dir45() {
  return unitSpec([](Rule &R, ASTContext &C) {
    Collector V(C);
    V.TraverseDecl(C.getTranslationUnitDecl());
    const SourceManager &SM = C.getSourceManager();
    for (auto *Group : {&V.Ordinary, &V.Tags}) {
      const auto &FileScope = (*Group)[nullptr];
      compareAmbiguous(R, SM, FileScope, FileScope, true);
      for (const auto &[Fn, Locals] : *Group) {
        if (!Fn) continue;
        compareAmbiguous(R, SM, Locals, Locals, true);
        compareAmbiguous(R, SM, Locals, FileScope, false);
      }
    }
  });
}

// ---- Dir 4.6 -------------------------------------------------------------
std::optional<SourceLocation> basicNumericLoc(TypeLoc TL) {
  while (!TL.isNull()) {
    if (auto Q = TL.getAs<QualifiedTypeLoc>()) TL = Q.getUnqualifiedLoc();
    else if (auto P = TL.getAs<PointerTypeLoc>()) TL = P.getPointeeLoc();
    else if (auto A = TL.getAs<ArrayTypeLoc>()) TL = A.getElementLoc();
    else if (auto Paren = TL.getAs<ParenTypeLoc>()) TL = Paren.getInnerLoc();
    else if (auto Attr = TL.getAs<AttributedTypeLoc>()) TL = Attr.getModifiedLoc();
    else if (auto F = TL.getAs<FunctionTypeLoc>()) TL = F.getReturnLoc();
    else break;
  }
  auto B = TL.getAs<BuiltinTypeLoc>();
  if (!B) return std::nullopt;
  const auto *T = B.getTypePtr();
  if (T->isVoidType() || T->isBooleanType() || !(T->isIntegerType() || T->isFloatingType())) return std::nullopt;
  return B.getBeginLoc();
}
Spec dir46() {
  return matcherSpec(declaratorDecl().bind("d"), [](Rule &R, const Result &Res) {
    const auto *D = Res.Nodes.getNodeAs<DeclaratorDecl>("d");
    if (!isUserDecl(D) || !D->getTypeSourceInfo()) return;
    if (const auto *F = dyn_cast<FieldDecl>(D); F && F->isBitField()) return;
    const FunctionDecl *Owner = nullptr;
    if (const auto *P = dyn_cast<ParmVarDecl>(D)) Owner = dyn_cast<FunctionDecl>(P->getDeclContext());
    if (const auto *F = dyn_cast<FunctionDecl>(D); F && F->isMain()) return;
    if (Owner && Owner->isMain()) return;
    if (auto Loc = basicNumericLoc(D->getTypeSourceInfo()->getTypeLoc()))
      R.report(*Loc, "basic numerical type used instead of a typedef that states size and signedness");
  });
}
Spec dir46Typedefs() {
  return matcherSpec(typedefDecl().bind("t"), [](Rule &R, const Result &Res) {
    const auto *D = Res.Nodes.getNodeAs<TypedefDecl>("t");
    if (!isUserDecl(D) || D->getName().find_first_of("0123456789") != llvm::StringRef::npos) return;
    if (auto Loc = basicNumericLoc(D->getTypeSourceInfo()->getTypeLoc()))
      R.report(*Loc, "typedef of a basic numerical type does not state size and signedness in its name");
  });
}

// ---- Dir 4.7 -------------------------------------------------------------
Spec dir47() {
  auto Shared = std::make_shared<llvm::StringSet<>>();
  Spec S = matcherSpec(callExpr(callee(functionDecl())).bind("call"), [Shared](Rule &R, const Result &Res) {
    if (Shared->empty()) {
      std::string List = R.option(
          "ErrorFunctions", "remove;rename;fclose;fflush;fseek;fsetpos;fgetpos;setvbuf;fputc;fputs;putc;putchar;"
                            "puts;fputwc;fputws;putwc;putwchar;ungetc;fprintf;printf;vfprintf;vprintf;snprintf;"
                            "vsnprintf;fscanf;scanf;sscanf;atexit;raise");
      llvm::SmallVector<llvm::StringRef, 32> Parts;
      llvm::StringRef(List).split(Parts, ';', -1, false);
      for (llvm::StringRef P : Parts) Shared->insert(P.trim());
    }
    const auto *Call = Res.Nodes.getNodeAs<CallExpr>("call");
    const auto *F = Call->getDirectCallee();
    if (!F || !Shared->contains(F->getName()) || !isUserLocation(*Res.SourceManager, Call->getBeginLoc())) return;
    if (valueUse(*Res.Context, Call) != Use::Used)
      R.report(Call->getBeginLoc(), ("error information returned by '" + F->getName() + "' is not tested").str());
  });
  return S;
}

// ---- Dir 4.8 -------------------------------------------------------------
class CompletenessUses : public RecursiveASTVisitor<CompletenessUses> {
public:
  explicit CompletenessUses(ASTContext &C) : C(C) {}
  void complete(QualType T) {
    T = T.getCanonicalType();
    while (const auto *A = C.getAsArrayType(T)) T = A->getElementType().getCanonicalType();
    if (const auto *Rec = T->getAsRecordDecl()) Needed.insert(Rec->getCanonicalDecl());
  }
  void pointer(QualType T) {
    T = T.getCanonicalType();
    bool ViaPointer = false;
    while (true) {
      if (const auto *P = T->getAs<PointerType>()) {
        T = P->getPointeeType().getCanonicalType();
        ViaPointer = true;
      } else if (const auto *A = C.getAsArrayType(T))
        T = A->getElementType().getCanonicalType();
      else
        break;
    }
    if (const auto *Rec = T->getAsRecordDecl(); Rec && ViaPointer) Pointed.insert(Rec->getCanonicalDecl());
  }
  bool VisitMemberExpr(MemberExpr *M) {
    if (const auto *F = dyn_cast<FieldDecl>(M->getMemberDecl())) Needed.insert(F->getParent()->getCanonicalDecl());
    return true;
  }
  bool VisitDeclaratorDecl(DeclaratorDecl *D) {
    if (isa<FunctionDecl>(D)) return true;
    complete(D->getType());
    if (C.getSourceManager().isInMainFile(D->getLocation())) pointer(D->getType());
    return true;
  }
  bool VisitUnaryExprOrTypeTraitExpr(UnaryExprOrTypeTraitExpr *E) {
    complete(E->getTypeOfArgument());
    return true;
  }
  bool VisitCompoundLiteralExpr(CompoundLiteralExpr *E) {
    complete(E->getType());
    return true;
  }
  bool VisitUnaryOperator(UnaryOperator *U) {
    if (U->getOpcode() == UO_Deref) complete(U->getType());
    return true;
  }
  ASTContext &C;
  std::set<const Decl *> Needed, Pointed;
};
Spec dir48() {
  return unitSpec([](Rule &R, ASTContext &C) {
    CompletenessUses V(C);
    V.TraverseDecl(C.getTranslationUnitDecl());
    const SourceManager &SM = C.getSourceManager();
    for (const Decl *D : C.getTranslationUnitDecl()->decls()) {
      const auto *Rec = dyn_cast<RecordDecl>(D);
      if (!Rec || !Rec->isThisDeclarationADefinition() || !isUserDecl(Rec) || SM.isInMainFile(Rec->getLocation()))
        continue;
      const Decl *Key = Rec->getCanonicalDecl();
      if (V.Pointed.count(Key) && !V.Needed.count(Key))
        R.report(Rec->getLocation(), "structure or union implementation is visible but only used through pointers in "
                                     "this translation unit; consider an opaque type");
    }
  });
}

// ---- Dir 4.9 -------------------------------------------------------------
struct MacroUse {
  const MacroInfo *Info = nullptr;
  SourceLocation Loc;
  bool Disqualified = false;
};
class FunctionMacros final : public PPCallbacks {
public:
  FunctionMacros(Rule &R, Preprocessor &PP) : R(R), PP(PP) {}
  void MacroDefined(const Token &Name, const MacroDirective *MD) override {
    const MacroInfo *MI = MD->getMacroInfo();
    if (!MI->isFunctionLike() || MI->isBuiltinMacro() || MI->tokens_empty() ||
        !isUserLocation(PP.getSourceManager(), Name.getLocation()))
      return;
    MacroUse U{MI, Name.getLocation(), false};
    std::set<const IdentifierInfo *> Seen;
    for (const Token &T : MI->tokens()) {
      if (T.isOneOf(tok::hash, tok::hashhash, tok::l_brace, tok::r_brace, tok::semi, tok::equal) ||
          T.isOneOf(tok::plusplus, tok::minusminus) || (T.getIdentifierInfo() && T.getIdentifierInfo()->isKeyword(PP.getLangOpts())))
        U.Disqualified = true;
      if (T.getIdentifierInfo()) Seen.insert(T.getIdentifierInfo());
      if (T.isOneOf(tok::plusequal, tok::minusequal, tok::starequal, tok::slashequal, tok::percentequal) ||
          T.isOneOf(tok::ampequal, tok::pipeequal, tok::caretequal, tok::lesslessequal, tok::greatergreaterequal))
        U.Disqualified = true;
    }
    for (const IdentifierInfo *P : MI->params())
      if (!Seen.count(P)) U.Disqualified = true;
    Macros[MI] = U;
  }
  void MacroExpands(const Token &, const MacroDefinition &MD, SourceRange, const MacroArgs *Args) override {
    auto Found = Macros.find(MD.getMacroInfo());
    if (Found == Macros.end()) return;
    if (PP.isParsingIfOrElifDirective()) Found->second.Disqualified = true;
    if (!Args) return;
    for (unsigned I = 0; I < Args->getNumMacroArguments(); ++I) {
      const Token *T = Args->getUnexpArgument(I);
      unsigned Count = 0;
      bool Punctuator = false;
      for (; T->isNot(tok::eof); ++T, ++Count)
        Punctuator = !T->isAnyIdentifier() && !T->isLiteral() && T->isNot(tok::l_paren) && T->isNot(tok::r_paren);
      bool TypeName = false;
      for (const Token *U = Args->getUnexpArgument(I); U->isNot(tok::eof); ++U)
        if (U->getIdentifierInfo() && U->getIdentifierInfo()->isKeyword(PP.getLangOpts())) TypeName = true;
      if ((Count == 1 && Punctuator) || TypeName) Found->second.Disqualified = true;
    }
  }
  void EndOfMainFile() override {
    for (const auto &[MI, U] : Macros)
      if (!U.Disqualified) R.report(U.Loc, "function-like macro could be replaced by a function");
  }

private:
  Rule &R;
  Preprocessor &PP;
  std::map<const MacroInfo *, MacroUse> Macros;
};
Spec dir49() {
  Spec S;
  S.Preprocessor = [](Rule &R, Preprocessor &PP, const SourceManager &) {
    PP.addPPCallbacks(std::make_unique<FunctionMacros>(R, PP));
  };
  return S;
}

// ---- Dir 4.10 ------------------------------------------------------------
class IncludeGuards final : public PPCallbacks {
public:
  IncludeGuards(Rule &R, Preprocessor &PP) : R(R), PP(PP) {}
  void FileChanged(SourceLocation Loc, FileChangeReason Reason, SrcMgr::CharacteristicKind Kind, FileID) override {
    const SourceManager &SM = PP.getSourceManager();
    if (Reason != EnterFile || Kind != SrcMgr::C_User || Loc.isInvalid()) return;
    FileID Id = SM.getFileID(Loc);
    if (Id == SM.getMainFileID()) return;
    if (auto Entry = SM.getFileEntryRefForID(Id)) Headers.emplace(*Entry, Id);
  }
  void EndOfMainFile() override {
    const SourceManager &SM = PP.getSourceManager();
    HeaderSearch &HS = PP.getHeaderSearchInfo();
    for (const auto &[Entry, Id] : Headers) {
      SourceLocation Start = SM.getLocForStartOfFile(Id);
      HeaderFileInfo &Info = HS.getFileInfo(Entry);
      if (Info.isPragmaOnce) {
        R.report(Start, "header uses #pragma once instead of an #ifndef/#define include guard");
        continue;
      }
      const IdentifierInfo *Guard = Info.getControllingMacro(PP.getExternalSource());
      if (!HS.isFileMultipleIncludeGuarded(Entry) || !Guard)
        R.report(Start, "header is not protected against repeated inclusion");
      else if (!PP.isMacroDefined(Guard))
        R.report(Start, ("include guard '" + Guard->getName() + "' is tested but never defined").str());
    }
  }

private:
  Rule &R;
  Preprocessor &PP;
  std::map<FileEntryRef, FileID> Headers;
};
Spec dir410() {
  Spec S;
  S.Preprocessor = [](Rule &R, Preprocessor &PP, const SourceManager &) {
    PP.addPPCallbacks(std::make_unique<IncludeGuards>(R, PP));
  };
  return S;
}

// ---- Dir 4.12 ------------------------------------------------------------
Spec dir412() {
  return matcherSpec(declRefExpr(to(functionDecl(hasAnyName("malloc", "calloc", "realloc", "free", "aligned_alloc",
                                                            "alloca", "__builtin_alloca"))))
                         .bind("ref"),
                     [](Rule &R, const Result &Res) {
                       const auto *Ref = Res.Nodes.getNodeAs<DeclRefExpr>("ref");
                       if (isUserLocation(*Res.SourceManager, Ref->getLocation()))
                         R.report(Ref->getLocation(), ("dynamic memory function '" +
                                                       Ref->getDecl()->getName() + "' is used").str());
                     });
}
} // namespace

void addDirectives(Table &T) {
  T.push_back({"misra-c2012-dir-1-1", dir11});
  T.push_back({"misra-c2012-dir-4-2", dir42});
  T.push_back({"misra-c2012-dir-4-3", dir43});
  T.push_back({"misra-c2012-dir-4-4", dir44});
  T.push_back({"misra-c2012-dir-4-5", dir45});
  T.push_back({"misra-c2012-dir-4-6", [] {
                 Spec Decls = dir46(), Typedefs = dir46Typedefs();
                 Spec S;
                 S.Matchers = [Decls, Typedefs](MatchFinder &F, Rule &R) {
                   Decls.Matchers(F, R);
                   Typedefs.Matchers(F, R);
                 };
                 S.Check = [Decls, Typedefs](Rule &R, const Result &Res) {
                   if (Res.Nodes.getNodeAs<TypedefDecl>("t")) Typedefs.Check(R, Res);
                   else Decls.Check(R, Res);
                 };
                 return S;
               }});
  T.push_back({"misra-c2012-dir-4-7", dir47});
  T.push_back({"misra-c2012-dir-4-8", dir48});
  T.push_back({"misra-c2012-dir-4-9", dir49});
  T.push_back({"misra-c2012-dir-4-10", dir410});
  T.push_back({"misra-c2012-dir-4-12", dir412});
}
} // namespace safety::misra
