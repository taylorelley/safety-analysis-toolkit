// SPDX-License-Identifier: Apache-2.0
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Index/USRGeneration.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/JSON.h"
#include "llvm/Support/raw_ostream.h"
using namespace clang;
static llvm::cl::OptionCategory Category("safety source index");
static std::string usr(const Decl *D) {
  llvm::SmallString<128> Buffer;
  if (!D || index::generateUSRForDecl(D, Buffer)) return "";
  return std::string(Buffer);
}
class Visitor : public RecursiveASTVisitor<Visitor> {
  ASTContext &Context;
public:
  explicit Visitor(ASTContext &Context) : Context(Context) {}
  bool VisitFunctionDecl(FunctionDecl *D) {
    const auto &SM = Context.getSourceManager();
    auto Location = SM.getExpansionLoc(D->getLocation());
    if (D->isImplicit() || Location.isInvalid() || SM.isInSystemHeader(Location)) return true;
    auto Presumed = SM.getPresumedLoc(Location);
    if (Presumed.isInvalid()) return true;
    std::string Comment;
    if (auto *C = Context.getRawCommentForAnyRedecl(D)) Comment = C->getRawText(SM).str();
    auto ID = usr(D->getCanonicalDecl());
    llvm::json::Object Record{{"usr", ID}, {"name", D->getQualifiedNameAsString()},
      {"file", Presumed.getFilename()}, {"line", Presumed.getLine()},
      {"column", Presumed.getColumn()}, {"definition", D->isThisDeclarationADefinition()},
      {"comment", Comment}, {"macro_expansion", D->getLocation().isMacroID()},
      {"template_parent", usr(D->getPrimaryTemplate())}};
    llvm::outs() << llvm::formatv("{0}\n", llvm::json::Value(std::move(Record)));
    return true;
  }
};
class Consumer : public ASTConsumer {
  void HandleTranslationUnit(ASTContext &Context) override {
    Visitor V(Context); V.TraverseDecl(Context.getTranslationUnitDecl());
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
  return Tool.run(tooling::newFrontendActionFactory<Action>().get());
}
