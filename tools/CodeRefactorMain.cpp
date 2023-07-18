#include "CodeRefactor.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Refactoring.h"
#include "llvm/Support/CommandLine.h"

using namespace llvm;
using namespace clang;

static cl::OptionCategory CodeRefactorCategory("ct-code-refactor options");

/**
 * Main的Act类为何不复用.so的Act类?   本类 CrPluginAction  与 类 CrAddPluginAction 是重复的吗？
 */
class CrPluginAction : public PluginASTAction {
public:
  explicit CrPluginAction(){};
  bool ParseArgs(const CompilerInstance &CI,
                 const std::vector<std::string> &args) override {
    return true;
  }

   void EndSourceFileAction() override {
//     rewriter
//         .getEditBuffer(rewriter.getSourceMgr().getMainFileID())
//         .write(llvm::outs()); //这里输出原始的?
   }

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef file) override {//file:"/pubx/clang_plugin_demo/clang-tutor/test/CodeRefactor_Class.cpp"
    rewriter.setSourceMgr(CI.getSourceManager(),
                          CI.getLangOpts());
    return std::make_unique<CrASTConsumer>(
            rewriter );
  }

private:
  Rewriter rewriter;
};

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//
int main(int Argc, const char **Argv) {
  Expected<tooling::CommonOptionsParser> eOptParser =
      clang::tooling::CommonOptionsParser::create(Argc, Argv,
                                                  CodeRefactorCategory);
  if (auto error = eOptParser.takeError()) {
    errs() << "Problem constructing CommonOptionsParser "
           << toString(std::move(error)) << '\n';
    return EXIT_FAILURE;
  }
  clang::tooling::RefactoringTool tool(eOptParser->getCompilations(),
                                       eOptParser->getSourcePathList());

  return tool.runAndSave(
      clang::tooling::newFrontendActionFactory<CrPluginAction>()
          .get());
}
