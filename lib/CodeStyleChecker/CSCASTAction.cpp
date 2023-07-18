#include "CodeStyleChecker.h"

#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"

using namespace clang;

class CSCASTAction : public PluginASTAction {
public:
    std::unique_ptr<ASTConsumer>
    CreateASTConsumer(CompilerInstance &Compiler,
                      llvm::StringRef InFile) override {
      return std::make_unique<CodeStyleCheckerASTConsumer>(
              &Compiler.getASTContext(), MainTuOnly, Compiler.getSourceManager());
    }

    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string> &Args) override {
      for (StringRef Arg : Args) {
        if (Arg.startswith("-main-tu-only="))
          MainTuOnly =
                  Arg.substr(strlen("-main-tu-only=")).equals_insensitive("true");
        else if (Arg.startswith("-help"))
          PrintHelp(llvm::errs());
        else
          return false;
      }

      return true;
    }

    void PrintHelp(llvm::raw_ostream &ros) {
      ros << "Help for CodeStyleChecker plugin goes here\n";
    }

private:
    bool MainTuOnly = true;
};

static clang::FrontendPluginRegistry::Add<CSCASTAction>
        X(/*Name=*/"CSC",
        /*Description=*/"Checks whether class, variable and function names "
                        "adhere to LLVM's guidelines");
