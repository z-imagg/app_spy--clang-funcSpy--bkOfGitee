#include "CodeRefactor/CrASTConsumer.h"

#include "clang/AST/Expr.h"
#include "clang/AST/ExprCXX.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Tooling/Refactoring/Rename/RenamingAction.h"
#include "llvm/Support/raw_ostream.h"
#include <string>

using namespace clang;
using namespace ast_matchers;



//-----------------------------------------------------------------------------
// FrontendAction
//-----------------------------------------------------------------------------
class CrAddPluginAction : public PluginASTAction {
public:
    bool ParseArgs(const CompilerInstance &CI,
                   const std::vector<std::string> &Args) override {

      return true;
    }

    // Returns our ASTConsumer per translation unit.
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                   StringRef file) override {
      RewriterForCodeRefactor.setSourceMgr(CI.getSourceManager(),
                                           CI.getLangOpts());
      return std::make_unique<CrASTConsumer>(
              RewriterForCodeRefactor);
    }

private:
    Rewriter RewriterForCodeRefactor;
    std::string ClassName;
    std::string OldName;
    std::string NewName;
};

//-----------------------------------------------------------------------------
// Registration
//-----------------------------------------------------------------------------
static FrontendPluginRegistry::Add<CrAddPluginAction>
        X(/*Name=*/"CodeRefactor",
        /*Desc=*/"Change the name of a class method");
