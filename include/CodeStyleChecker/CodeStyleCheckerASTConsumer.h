#include <clang/Rewrite/Core/Rewriter.h>
#include <iostream>
#include "clang/AST/ASTConsumer.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"


#include "CodeStyleChecker/CodeStyleCheckerVisitor.h"

/**
 * generate by :  https://chat.chatgptdemo.net/
 */
class CallExprMatcher : public clang::ast_matchers::MatchFinder::MatchCallback {
public:
    CallExprMatcher() {}

    void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override {
    clang::SourceManager& SM=Result.Context->getSourceManager();
    if (const clang::CallExpr *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
      const clang::FunctionDecl* callee=CE->getDirectCallee();
      if(!callee){
        return;
      }

      clang::SourceLocation beginLoc=CE->getBeginLoc();
      clang::FileID fileId = SM.getFileID(beginLoc);

      std::string funcName=callee->getNameInfo().getName().getAsString();
      //能找到 时钟滴答 函数调用
      //若已经有时钟函数调用
      if(funcName==CodeStyleCheckerVisitor::funcName_TCTick){
        //若已经有时钟函数调用，则标记为已处理
        CodeStyleCheckerVisitor::fileInsertedIncludeStmt.insert(fileId);
      }
      /*llvm::outs() << "Matched CallExpr at "
                   << CE->getBeginLoc().printToString(SM)
                   << "\n";*/
    }
  }

};

//-----------------------------------------------------------------------------
// ASTConsumer
//-----------------------------------------------------------------------------
class CodeStyleCheckerASTConsumer : public clang::ASTConsumer {
public:
    //Rewriter:3:  Action将Rewriter传递给Consumer
    explicit CodeStyleCheckerASTConsumer(clang::Rewriter &R, clang::ASTContext *Context,
                                         clang::SourceManager &SM)
            //Rewriter:4:  Consumer将Rewriter传递给Visitor
            : Visitor(R, Context),
            SM(SM)  {}


    virtual void HandleTranslationUnit(clang::ASTContext &Ctx) override{
      clang::FileID mainFileId = SM.getMainFileID();
      const clang::LangOptions & langOpts = Visitor.mRewriter.getLangOpts();
      //时钟函数只插入一次，不重复插入：
      //若已经有时钟函数声明，则标记为已处理，且直接返回，不做任何处理。
      //包含了时钟头文件 会有时钟函数声明。 其实是想问 是否有 时钟函数调用，但函数调用 比 声明 难找。
      //所以说 有漏洞：如果一个客户源文件，没有包含时钟头文件，但是调用了时钟函数 ，是会每次都被插入时钟语句。
      {
        clang::ast_matchers::MatchFinder Finder;
        CallExprMatcher Matcher;
        Finder.addMatcher(clang::ast_matchers::callExpr().bind("callExpr"), &Matcher);

        Ctx.getTranslationUnitDecl();
        Finder.matchAST(Ctx);//这里应该会循环调完 CallExprMatcher.run

        if(CodeStyleCheckerVisitor::fileInsertedIncludeStmt.count(mainFileId)>0){
          //若已经有时钟函数声明，则标记为已处理，且直接返回，不做任何处理。
          return;
        }

      }

      auto filePath=SM.getFileEntryForID(SM.getMainFileID())->getName().str();
      std::cout<<"HandleTranslationUnit__filepath:"<<filePath<<std::endl;

      clang::TranslationUnitDecl* translationUnitDecl=Ctx.getTranslationUnitDecl();
      Visitor.TraverseDecl(translationUnitDecl);


//      Visitor.mRewriter.getEditBuffer(SM.getMainFileID())
//              .write(llvm::outs());

        //不在这里写出修改，而是到 函数 EndSourceFileAction 中去 写出修改
      Visitor.mRewriter.overwriteChangedFiles();//修改会影响原始文件


    }

private:
    CodeStyleCheckerVisitor Visitor;
    clang::SourceManager &SM;
    // Should this plugin be only run on the main translation unit?
};
