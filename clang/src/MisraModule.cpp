// SPDX-License-Identifier: Apache-2.0
#include "clang-tidy/ClangTidyCheck.h"
#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include <map>

namespace safety {
// Original project-policy check. No MISRA rule identifier is claimed until a
// licensed-standard review validates the mapping and its scope.
class ProhibitedGoto final : public clang::tidy::ClangTidyCheck {
public:
  ProhibitedGoto(llvm::StringRef Name, clang::tidy::ClangTidyContext *Context)
      : ClangTidyCheck(Name, Context) {}
  void registerMatchers(clang::ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;
    Finder->addMatcher(gotoStmt().bind("goto"), this);
  }
  void check(const clang::ast_matchers::MatchFinder::MatchResult &Result) override {
    const auto *Statement = Result.Nodes.getNodeAs<clang::GotoStmt>("goto");
    diag(Statement->getGotoLoc(), "goto statement requires project policy review");
  }
};
class UnsafeLibrary final : public clang::tidy::ClangTidyCheck {
public:
  using ClangTidyCheck::ClangTidyCheck;
  void registerMatchers(clang::ast_matchers::MatchFinder *Finder) override {
    using namespace clang::ast_matchers;
    Finder->addMatcher(callExpr(callee(functionDecl(hasAnyName("::gets", "::longjmp")))).bind("call"), this);
  }
  void check(const clang::ast_matchers::MatchFinder::MatchResult &R) override {
    const auto *Call = R.Nodes.getNodeAs<clang::CallExpr>("call");
    const auto *Callee = Call->getDirectCallee();
    if (Callee && Callee->isExternC()) diag(Call->getExprLoc(), "call to %0 requires project library-policy review") << Callee->getName();
  }
};
class RepeatedMacro final : public clang::tidy::ClangTidyCheck {
  class Callback final : public clang::PPCallbacks {
    RepeatedMacro &Check;
  public:
    explicit Callback(RepeatedMacro &Check) : Check(Check) {}
    void MacroDefined(const clang::Token &Name, const clang::MacroDirective *Directive) override {
      const auto *Info = Directive->getMacroInfo();
      if (!Info->isFunctionLike() || Info->isBuiltinMacro()) return;
      std::map<const clang::IdentifierInfo *, unsigned> Counts;
      for (const auto *Parameter : Info->params()) Counts[Parameter] = 0;
      for (const auto &Token : Info->tokens()) {
        if (Token.is(clang::tok::identifier)) {
          auto Found = Counts.find(Token.getIdentifierInfo());
          if (Found != Counts.end()) ++Found->second;
        }
      }
      for (const auto &Entry : Counts) if (Entry.second > 1)
        Check.diag(Name.getLocation(), "macro parameter %0 occurs repeatedly; review evaluation safety") << Entry.first->getName();
    }
  };
public:
  using ClangTidyCheck::ClangTidyCheck;
  void registerPPCallbacks(const clang::SourceManager &, clang::Preprocessor *PP,
                           clang::Preprocessor *) override {
    PP->addPPCallbacks(std::make_unique<Callback>(*this));
  }
};
class SafetyModule final : public clang::tidy::ClangTidyModule {
  void addCheckFactories(clang::tidy::ClangTidyCheckFactories &Factories) override {
    Factories.registerCheck<ProhibitedGoto>("safety-prohibited-goto");
    Factories.registerCheck<UnsafeLibrary>("safety-unsafe-library-call");
    Factories.registerCheck<RepeatedMacro>("safety-repeated-macro-parameter");
  }
};
static clang::tidy::ClangTidyModuleRegistry::Add<SafetyModule>
    Registration("safety-module", "Original safety project-policy checks");
} // namespace safety
