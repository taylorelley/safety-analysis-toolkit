// SPDX-License-Identifier: Apache-2.0
// Shared infrastructure for the MISRA C:2012 guideline checks. Diagnostic
// wording is original; no guideline text from the standard is reproduced.
#pragma once
#include "clang-tidy/ClangTidyCheck.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Expr.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace safety::misra {
namespace am = clang::ast_matchers;
using Result = am::MatchFinder::MatchResult;

class Rule;
// Behaviour of one guideline check. A fresh Spec is built for every check
// instance so that callbacks can share state through captured pointers.
struct Spec {
  std::function<void(am::MatchFinder &, Rule &)> Matchers;
  std::function<void(Rule &, const Result &)> Check;
  std::function<void(Rule &, clang::Preprocessor &, const clang::SourceManager &)> Preprocessor;
  std::function<void(Rule &)> End;
};

class Rule final : public clang::tidy::ClangTidyCheck {
public:
  Rule(llvm::StringRef Name, clang::tidy::ClangTidyContext *Context, Spec S)
      : ClangTidyCheck(Name, Context), S(std::move(S)) {}
  bool isLanguageVersionSupported(const clang::LangOptions &L) const override { return !L.CPlusPlus; }
  void registerMatchers(am::MatchFinder *F) override {
    if (S.Matchers) S.Matchers(*F, *this);
  }
  void check(const Result &R) override {
    if (S.Check) S.Check(*this, R);
  }
  void registerPPCallbacks(const clang::SourceManager &SM, clang::Preprocessor *PP,
                           clang::Preprocessor *) override {
    if (S.Preprocessor) S.Preprocessor(*this, *PP, SM);
  }
  void onEndOfTranslationUnit() override {
    if (S.End) S.End(*this);
  }
  // Report at most once per location so overlapping traversals stay quiet.
  void report(clang::SourceLocation Loc, llvm::StringRef Message);
  std::string option(llvm::StringRef Name, llvm::StringRef Default) const { return Options.get(Name, Default).str(); }
  clang::ASTContext *Context = nullptr;

private:
  Spec S;
  std::vector<clang::SourceLocation> Reported;
};

using Factory = std::function<Spec()>;
using Table = std::vector<std::pair<const char *, Factory>>;
void addDirectives(Table &T);
void addRulesBasic(Table &T);
void addRulesDeclarations(Table &T);
void addRulesTypes(Table &T);
void addRulesExpressions(Table &T);
void addRulesControl(Table &T);
void addRulesPointers(Table &T);
void addRulesPreprocessor(Table &T);
void addRulesLibrary(Table &T);

// Spec for checks that bind a matcher and handle every match with `Run`.
template <typename M>
Spec matcherSpec(M Matcher, std::function<void(Rule &, const Result &)> Run) {
  Spec S;
  S.Matchers = [Matcher](am::MatchFinder &F, Rule &R) { F.addMatcher(Matcher, &R); };
  S.Check = [Run](Rule &R, const Result &Res) {
    R.Context = Res.Context;
    Run(R, Res);
  };
  return S;
}
// Spec for whole translation-unit analyses.
Spec unitSpec(std::function<void(Rule &, clang::ASTContext &)> Run);

// ---- helpers -------------------------------------------------------------
bool isUserLocation(const clang::SourceManager &SM, clang::SourceLocation L);
bool isUserDecl(const clang::Decl *D);
const clang::Stmt *parentStmt(clang::ASTContext &C, const clang::Stmt *S);
const clang::Decl *parentDecl(clang::ASTContext &C, const clang::Stmt *S);
// Strip parentheses only.
const clang::Expr *stripParens(const clang::Expr *E);
const clang::Expr *stripAll(const clang::Expr *E);

enum class Use { Discarded, VoidCast, Used };
// How the value of an expression is consumed by its context.
Use valueUse(clang::ASTContext &C, const clang::Expr *E);

struct Effects {
  unsigned IncDec = 0, Assign = 0, Calls = 0, VolatileRead = 0;
  unsigned total() const { return IncDec + Assign + Calls + VolatileRead; }
};
// Count potential side effects; `Unevaluated` controls descent into sizeof.
Effects effectsOf(const clang::Stmt *S, bool Unevaluated = true);
bool isVolatileRead(const clang::Expr *E);

// The variable named by an lvalue such as `x`, `x.m` or `(x)`.
const clang::VarDecl *namedVariable(const clang::Expr *E);
std::string objectKey(const clang::Expr *E);

std::optional<llvm::APSInt> integerConstant(const clang::Expr *E, clang::ASTContext &C);
bool isConstantExpression(const clang::Expr *E, clang::ASTContext &C);
// True when control can reach the end of the statement.
bool canCompleteNormally(const clang::Stmt *S, clang::ASTContext &C);
bool isNoReturnCall(const clang::Stmt *S);
std::string sourceText(const clang::SourceManager &SM, const clang::LangOptions &LO,
                       clang::SourceRange R);
bool rangeHasComment(const clang::SourceManager &SM, const clang::LangOptions &LO,
                     clang::SourceRange R);
// Name of the outermost-to-innermost macro chain at a location contains Name.
bool expandedFromMacro(const clang::SourceManager &SM, const clang::LangOptions &LO,
                       clang::SourceLocation L, llvm::StringRef Name);
std::string spelling(const clang::SourceManager &SM, const clang::LangOptions &LO,
                     clang::SourceLocation L);

// Invoke F for every comment in user files; the returned handler must outlive
// preprocessing of the translation unit.
std::unique_ptr<clang::CommentHandler>
onComments(clang::Preprocessor &PP, std::function<void(clang::SourceRange, llvm::StringRef)> F);

// ---- essential type model ------------------------------------------------
enum class Cat { None, Boolean, Character, Signed, Unsigned, Enum, Floating };
struct EType {
  Cat C = Cat::None;
  uint64_t Bits = 0;
  const clang::EnumDecl *Enum = nullptr;
  bool same(const EType &O) const { return C == O.C && (C != Cat::Enum || Enum == O.Enum); }
  bool arithmetic() const { return C != Cat::None; }
};
EType essentialOfType(clang::QualType T, clang::ASTContext &C);
EType essentialType(const clang::Expr *E, clang::ASTContext &C);
const char *categoryName(Cat C);
bool isComposite(const clang::Expr *E, clang::ASTContext &C);
// Assignments in the sense of the essential type rules (assignment,
// initialization, argument passing, return and case labels).
struct Assignment {
  const clang::Expr *Source;
  EType Target;
  clang::SourceLocation Loc;
  bool CaseLabel = false;
};
void forEachAssignment(clang::ASTContext &C, std::function<void(const Assignment &)> F);

// ---- library identifiers ---------------------------------------------------
// Identifiers and macro names that the C library reserves at file scope.
bool isLibraryIdentifier(llvm::StringRef Name, bool C99);
} // namespace safety::misra
