// SPDX-License-Identifier: Apache-2.0
#include "clang-tidy/ClangTidyCheck.h"
#include "clang-tidy/ClangTidyModule.h"
#include "clang-tidy/ClangTidyModuleRegistry.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

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
class SafetyModule final : public clang::tidy::ClangTidyModule {
  void addCheckFactories(clang::tidy::ClangTidyCheckFactories &Factories) override {
    Factories.registerCheck<ProhibitedGoto>("safety-prohibited-goto");
  }
};
static clang::tidy::ClangTidyModuleRegistry::Add<SafetyModule>
    Registration("safety-module", "Original safety project-policy checks");
} // namespace safety
