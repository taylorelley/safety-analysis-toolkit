// SPDX-License-Identifier: Apache-2.0
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/DeclTemplate.h"
#include <set>
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Index/USRGeneration.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/JSON.h"
#include "llvm/Support/raw_ostream.h"
using namespace clang;
static llvm::cl::OptionCategory Category("safety source index");
static llvm::cl::opt<std::string> DependencyOutput("dependencies", llvm::cl::desc("Write dependency JSON"), llvm::cl::cat(Category));
static std::set<std::string> Dependencies;
static std::string usr(const Decl *D) {
  llvm::SmallString<128> Buffer;
  if (!D || index::generateUSRForDecl(D, Buffer)) return "";
  return std::string(Buffer);
}
class Calls : public RecursiveASTVisitor<Calls> {
public:
  std::set<std::string> Targets;
  unsigned Unresolved = 0;
  bool TraverseLambdaExpr(LambdaExpr *) { ++Unresolved; return true; }
  bool VisitCallExpr(CallExpr *Call) {
    if (const auto *Member = llvm::dyn_cast<CXXMemberCallExpr>(Call))
      if (Member->getMethodDecl() && Member->getMethodDecl()->isVirtual()) ++Unresolved;
    if (auto *Target = Call->getDirectCallee()) {
      auto ID = usr(Target->getCanonicalDecl());
      if (!ID.empty()) Targets.insert(ID); else ++Unresolved;
    } else ++Unresolved;
    return true;
  }
  bool VisitCXXConstructExpr(CXXConstructExpr *Call) {
    auto ID = usr(Call->getConstructor()->getCanonicalDecl());
    if (!ID.empty()) Targets.insert(ID); else ++Unresolved;
    return true;
  }
};
class Visitor : public RecursiveASTVisitor<Visitor> {
  ASTContext &Context;
public:
  explicit Visitor(ASTContext &Context) : Context(Context) {}
  bool shouldVisitTemplateInstantiations() const { return true; }
  bool VisitFunctionDecl(FunctionDecl *D) {
    const auto &SM = Context.getSourceManager();
    auto Location = SM.getExpansionLoc(D->getLocation());
    if (D->isImplicit() || Location.isInvalid() || SM.isInSystemHeader(Location)) return true;
    auto Presumed = SM.getPresumedLoc(Location, false);
    if (Presumed.isInvalid()) return true;
    std::string Comment;
    if (auto *C = Context.getRawCommentForAnyRedecl(D)) Comment = C->getRawText(SM).str();
    auto ID = usr(D->getCanonicalDecl());
    Calls CallInfo;
    if (D->isThisDeclarationADefinition() && D->hasBody()) CallInfo.TraverseStmt(D->getBody());
    llvm::json::Array Callees;
    for (auto &Callee : CallInfo.Targets) Callees.push_back(Callee);
    auto *Primary = D->getPrimaryTemplate();
    std::string TemplateKind = Primary ? "specialization_or_instantiation" :
        (D->getDescribedFunctionTemplate() ? "primary" : "none");
    llvm::json::Object Record{{"usr", ID}, {"name", D->getQualifiedNameAsString()},
      {"file", Presumed.getFilename()}, {"line", Presumed.getLine()},
      {"column", Presumed.getColumn()}, {"definition", D->isThisDeclarationADefinition()},
      {"comment", Comment}, {"macro_expansion", D->getLocation().isMacroID()},
      {"template_parent", usr(Primary ? Primary->getTemplatedDecl()->getCanonicalDecl() : nullptr)},
      {"template_kind", TemplateKind}, {"calls", std::move(Callees)},
      {"unresolved_calls", CallInfo.Unresolved}};
    llvm::outs() << llvm::formatv("{0}\n", llvm::json::Value(std::move(Record)));
    return true;
  }
};
class Consumer : public ASTConsumer {
  void HandleTranslationUnit(ASTContext &Context) override {
    Visitor V(Context); V.TraverseDecl(Context.getTranslationUnitDecl());
    const auto &SM=Context.getSourceManager();
    for(auto I=SM.fileinfo_begin(); I!=SM.fileinfo_end(); ++I) Dependencies.insert(I->first.getName().str());
  }
};
class Action : public ASTFrontendAction {
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &, llvm::StringRef) override {
    return std::make_unique<Consumer>();
  }
};
int main(int argc, const char **argv) {
  auto Options = tooling::CommonOptionsParser::create(argc, argv, Category);
  if (!Options) { llvm::errs() << Options.takeError(); return 2; }
  tooling::ClangTool Tool(Options->getCompilations(), Options->getSourcePathList());
  Tool.appendArgumentsAdjuster(tooling::getInsertArgumentAdjuster("-fparse-all-comments", tooling::ArgumentInsertPosition::BEGIN));
  int Result=Tool.run(tooling::newFrontendActionFactory<Action>().get());
  if(!DependencyOutput.empty()) {std::error_code Error;llvm::raw_fd_ostream File(DependencyOutput, Error);if(Error){llvm::errs()<<Error.message();return 3;}llvm::json::Array Paths;for(const auto &Path:Dependencies) Paths.push_back(Path);File<<llvm::formatv("{0}\n",llvm::json::Value(std::move(Paths)));}
  return Result;
}
