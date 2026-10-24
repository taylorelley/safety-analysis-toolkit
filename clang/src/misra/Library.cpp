// SPDX-License-Identifier: Apache-2.0
// Rules 21.x (standard libraries) and 22.x (resources). Resource lifetime
// rules are also covered by the Clang static analyzer stream/malloc checkers.
#include "Misra.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Lex/MacroInfo.h"
#include "llvm/ADT/StringSet.h"
#include <map>

using namespace clang;
using namespace clang::ast_matchers;

namespace safety::misra {
namespace {
// ---- banned identifiers and headers (Rules 21.3-21.12) ----------------------
struct Banned {
  const char *Names;   // identifiers and macro names that must not be used
  const char *Headers; // headers whose inclusion is itself a use
  const char *What;
};
class BannedMacros final : public PPCallbacks {
public:
  BannedMacros(Rule &R, Preprocessor &PP, std::shared_ptr<llvm::StringSet<>> Names, std::vector<std::string> Headers,
               std::string What)
      : R(R), PP(PP), Names(std::move(Names)), Headers(std::move(Headers)), What(std::move(What)) {}
  void MacroExpands(const Token &Name, const MacroDefinition &, SourceRange, const MacroArgs *) override {
    if (Names->contains(Name.getIdentifierInfo()->getName()) && isUserLocation(PP.getSourceManager(), Name.getLocation()))
      R.report(Name.getLocation(), What + " '" + Name.getIdentifierInfo()->getName().str() + "' used");
  }
  void InclusionDirective(SourceLocation Hash, const Token &, StringRef FileName, bool, CharSourceRange, OptionalFileEntryRef,
                          StringRef, StringRef, const Module *, bool, SrcMgr::CharacteristicKind) override {
    if (!isUserLocation(PP.getSourceManager(), Hash)) return;
    for (const std::string &H : Headers)
      if (FileName == H) R.report(Hash, "<" + H + "> is included");
  }

private:
  Rule &R;
  Preprocessor &PP;
  std::shared_ptr<llvm::StringSet<>> Names;
  std::vector<std::string> Headers;
  std::string What;
};
Factory banned(Banned B) {
  return [B] {
    auto Names = std::make_shared<llvm::StringSet<>>();
    llvm::SmallVector<llvm::StringRef, 32> Parts;
    llvm::StringRef(B.Names).split(Parts, ' ', -1, false);
    for (llvm::StringRef P : Parts) Names->insert(P);
    std::vector<std::string> Headers;
    llvm::SmallVector<llvm::StringRef, 4> HeaderParts;
    llvm::StringRef(B.Headers).split(HeaderParts, ' ', -1, false);
    for (llvm::StringRef H : HeaderParts) Headers.push_back(H.str());
    std::string What = B.What;
    Spec S;
    S.Matchers = [](MatchFinder &F, Rule &R) {
      F.addMatcher(declRefExpr().bind("ref"), &R);
      F.addMatcher(typeLoc(loc(qualType(hasDeclaration(namedDecl().bind("type"))))).bind("tl"), &R);
    };
    S.Check = [Names, What](Rule &R, const Result &Res) {
      const NamedDecl *D = nullptr;
      SourceLocation L;
      if (const auto *Ref = Res.Nodes.getNodeAs<DeclRefExpr>("ref")) D = Ref->getDecl(), L = Ref->getLocation();
      if (const auto *TL = Res.Nodes.getNodeAs<TypeLoc>("tl")) D = Res.Nodes.getNodeAs<NamedDecl>("type"), L = TL->getBeginLoc();
      if (!D || !D->getIdentifier() || !Names->contains(D->getName())) return;
      // Library declarations only; a project's own function of the same name is reported by Rule 21.2.
      if (!Res.SourceManager->isInSystemHeader(D->getLocation()) && !D->isImplicit()) return;
      if (isUserLocation(*Res.SourceManager, L) && !Res.SourceManager->isMacroBodyExpansion(L))
        R.report(Res.SourceManager->getExpansionLoc(L), What + " '" + D->getName().str() + "' used");
    };
    S.Preprocessor = [Names, Headers, What](Rule &R, Preprocessor &PP, const SourceManager &) {
      PP.addPPCallbacks(std::make_unique<BannedMacros>(R, PP, Names, Headers, What));
    };
    return S;
  };
}

// ---- Rule 21.1 ------------------------------------------------------------------
bool reservedMacro(llvm::StringRef Name, bool C99) {
  if (Name == "NDEBUG") return false;
  return Name.starts_with("_") || isLibraryIdentifier(Name, C99);
}
class ReservedMacros final : public PPCallbacks {
public:
  ReservedMacros(Rule &R, Preprocessor &PP) : R(R), PP(PP) {}
  void check(const Token &Name, const char *Directive) {
    if (!isUserLocation(PP.getSourceManager(), Name.getLocation())) return;
    if (reservedMacro(Name.getIdentifierInfo()->getName(), PP.getLangOpts().C99))
      R.report(Name.getLocation(), std::string(Directive) + " applied to a reserved identifier or macro name");
  }
  void MacroDefined(const Token &Name, const MacroDirective *) override { check(Name, "#define"); }
  void MacroUndefined(const Token &Name, const MacroDefinition &, const MacroDirective *) override { check(Name, "#undef"); }

private:
  Rule &R;
  Preprocessor &PP;
};

// ---- Rules 22.3 / 22.4 --------------------------------------------------------------
struct Open {
  std::string Path, Mode;
  SourceLocation Loc;
};
std::optional<Open> fopenCall(const Expr *E) {
  const auto *Call = dyn_cast_or_null<CallExpr>(stripAll(E));
  if (!Call || !Call->getDirectCallee() || Call->getDirectCallee()->getName() != "fopen" || Call->getNumArgs() < 2)
    return std::nullopt;
  const auto *Path = dyn_cast<StringLiteral>(stripAll(Call->getArg(0)));
  const auto *Mode = dyn_cast<StringLiteral>(stripAll(Call->getArg(1)));
  if (!Path || !Mode) return std::nullopt;
  return Open{Path->getString().str(), Mode->getString().str(), Call->getBeginLoc()};
}
bool writable(llvm::StringRef Mode) { return Mode.contains('w') || Mode.contains('a') || Mode.contains('+'); }
class Streams : public RecursiveASTVisitor<Streams> {
public:
  // Stream variables opened in the current function with a literal mode.
  std::map<const VarDecl *, Open> Opened;
  std::vector<Open> All;
  void bind(const VarDecl *V, const Expr *Init) {
    if (auto O = fopenCall(Init)) {
      All.push_back(*O);
      if (V) Opened[V] = *O;
    }
  }
  bool VisitVarDecl(VarDecl *V) {
    if (V->hasInit()) bind(V, V->getInit());
    return true;
  }
  bool VisitBinaryOperator(BinaryOperator *B) {
    if (B->getOpcode() == BO_Assign) bind(namedVariable(B->getLHS()), B->getRHS());
    return true;
  }
  bool VisitCallExpr(CallExpr *Call) {
    if (const auto *F = Call->getDirectCallee()) Calls.push_back({F->getName().str(), Call});
    return true;
  }
  std::vector<std::pair<std::string, const CallExpr *>> Calls;
};
int streamArgument(llvm::StringRef Name) {
  static const std::map<std::string, int> Writers = {
      {"fprintf", 0}, {"vfprintf", 0}, {"fputc", 1}, {"fputs", 1}, {"putc", 1}, {"fwrite", 3}, {"fputwc", 1},
      {"fputws", 1},  {"putwc", 1},    {"fwprintf", 0}, {"vfwprintf", 0}};
  auto It = Writers.find(Name.str());
  return It == Writers.end() ? -1 : It->second;
}
bool fileType(QualType T) {
  T = T.getUnqualifiedType();
  while (const auto *TT = T->getAs<TypedefType>()) {
    if (TT->getDecl()->getName() == "FILE") return true;
    T = TT->getDecl()->getUnderlyingType();
  }
  return false;
}
} // namespace

void addRulesLibrary(Table &T) {
  T.push_back({"misra-c2012-rule-21-1", [] {
                 Spec S;
                 S.Preprocessor = [](Rule &R, Preprocessor &PP, const SourceManager &) {
                   PP.addPPCallbacks(std::make_unique<ReservedMacros>(R, PP));
                 };
                 return S;
               }});
  T.push_back({"misra-c2012-rule-21-2", [] {
                 return matcherSpec(namedDecl().bind("d"), [](Rule &R, const Result &Res) {
                   const auto *D = Res.Nodes.getNodeAs<NamedDecl>("d");
                   if (!isUserDecl(D) || !D->getIdentifier()) return;
                   llvm::StringRef Name = D->getName();
                   bool FileScope = D->getDeclContext()->isFileContext() && !isa<FieldDecl>(D);
                   bool Reserved = Name.starts_with("_") ||
                                   (FileScope && isa<VarDecl, FunctionDecl, TypedefNameDecl, TagDecl>(D) &&
                                    isLibraryIdentifier(Name, Res.Context->getLangOpts().C99));
                   if (Reserved) R.report(D->getLocation(), "reserved identifier '" + Name.str() + "' is declared");
                 });
               }});
  T.push_back({"misra-c2012-rule-21-3", banned({"calloc malloc realloc free", "", "memory allocation function"})});
  T.push_back({"misra-c2012-rule-21-4", banned({"setjmp longjmp jmp_buf _setjmp __sigsetjmp", "setjmp.h", "<setjmp.h> facility"})});
  T.push_back({"misra-c2012-rule-21-5", banned({"signal raise sig_atomic_t SIG_DFL SIG_ERR SIG_IGN SIGABRT SIGFPE SIGILL SIGINT SIGSEGV SIGTERM",
                                                "signal.h", "<signal.h> facility"})});
  T.push_back({"misra-c2012-rule-21-6",
               banned({"remove rename tmpfile tmpnam fclose fflush fopen freopen setbuf setvbuf fprintf fscanf printf scanf "
                       "snprintf sprintf sscanf vfprintf vfscanf vprintf vscanf vsnprintf vsprintf vsscanf fgetc fgets fputc fputs "
                       "getc getchar gets putc putchar puts ungetc fread fwrite fgetpos fseek fsetpos ftell rewind clearerr feof "
                       "ferror perror fwprintf fwscanf swprintf swscanf vfwprintf vfwscanf vswprintf vswscanf vwprintf vwscanf "
                       "wprintf wscanf fgetwc fgetws fputwc fputws fwide getwc getwchar putwc putwchar ungetwc",
                       "", "standard input/output function"})});
  T.push_back({"misra-c2012-rule-21-7", banned({"atof atoi atol atoll", "", "string conversion function"})});
  T.push_back({"misra-c2012-rule-21-8", banned({"abort exit getenv system", "", "termination or environment function"})});
  T.push_back({"misra-c2012-rule-21-9", banned({"bsearch qsort", "", "search or sort function"})});
  T.push_back({"misra-c2012-rule-21-10", banned({"time clock difftime mktime asctime ctime gmtime localtime strftime wcsftime "
                                                 "clock_t time_t tm CLOCKS_PER_SEC",
                                                 "time.h", "time and date facility"})});
  T.push_back({"misra-c2012-rule-21-11", banned({"", "tgmath.h", "<tgmath.h> facility"})});
  T.push_back({"misra-c2012-rule-21-12", banned({"feclearexcept fegetexceptflag feraiseexcept fesetexceptflag fetestexcept "
                                                 "FE_INEXACT FE_DIVBYZERO FE_UNDERFLOW FE_OVERFLOW FE_INVALID FE_ALL_EXCEPT",
                                                 "", "floating-point exception facility"})});
  T.push_back({"misra-c2012-rule-22-3", [] {
                 return matcherSpec(functionDecl(isDefinition()).bind("f"), [](Rule &R, const Result &Res) {
                   const auto *F = Res.Nodes.getNodeAs<FunctionDecl>("f");
                   if (!isUserDecl(F)) return;
                   Streams V;
                   V.TraverseStmt(F->getBody());
                   for (size_t I = 0; I < V.All.size(); ++I)
                     for (size_t J = 0; J < I; ++J)
                       if (V.All[I].Path == V.All[J].Path && (writable(V.All[I].Mode) || writable(V.All[J].Mode)))
                         R.report(V.All[I].Loc, "file may be open for reading and writing on different streams");
                 });
               }});
  T.push_back({"misra-c2012-rule-22-4", [] {
                 return matcherSpec(functionDecl(isDefinition()).bind("f"), [](Rule &R, const Result &Res) {
                   const auto *F = Res.Nodes.getNodeAs<FunctionDecl>("f");
                   if (!isUserDecl(F)) return;
                   Streams V;
                   V.TraverseStmt(F->getBody());
                   for (const auto &[Name, Call] : V.Calls) {
                     int Index = streamArgument(Name);
                     if (Index < 0 || Index >= static_cast<int>(Call->getNumArgs())) continue;
                     const VarDecl *Stream = namedVariable(Call->getArg(Index));
                     auto It = V.Opened.find(Stream);
                     if (It != V.Opened.end() && !writable(It->second.Mode))
                       R.report(Call->getBeginLoc(), "write to a stream opened read-only");
                   }
                 });
               }});
  T.push_back({"misra-c2012-rule-22-5", [] {
                 Spec S;
                 S.Matchers = [](MatchFinder &F, Rule &R) {
                   F.addMatcher(unaryOperator(hasOperatorName("*")).bind("e"), &R);
                   F.addMatcher(memberExpr(isArrow()).bind("e"), &R);
                   F.addMatcher(arraySubscriptExpr().bind("e"), &R);
                 };
                 S.Check = [](Rule &R, const Result &Res) {
                   const auto *E = Res.Nodes.getNodeAs<Expr>("e");
                   if (!isUserLocation(*Res.SourceManager, E->getExprLoc())) return;
                   QualType Pointer;
                   if (const auto *U = dyn_cast<UnaryOperator>(E)) Pointer = U->getSubExpr()->IgnoreParenImpCasts()->getType();
                   if (const auto *M = dyn_cast<MemberExpr>(E)) Pointer = M->getBase()->IgnoreParenImpCasts()->getType();
                   if (const auto *A = dyn_cast<ArraySubscriptExpr>(E)) Pointer = A->getBase()->IgnoreParenImpCasts()->getType();
                   const auto *P = Pointer.isNull() ? nullptr : Pointer->getAs<PointerType>();
                   if (P && fileType(P->getPointeeType())) R.report(E->getExprLoc(), "pointer to a FILE object is dereferenced");
                 };
                 return S;
               }});
}
} // namespace safety::misra
