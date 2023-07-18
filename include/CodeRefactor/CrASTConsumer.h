
#ifndef CLANG_TUTOR_CRASTCONSUMER_H
#define CLANG_TUTOR_CRASTCONSUMER_H

#include "clang/AST/ASTConsumer.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/FixItRewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"


#include "CodeRefactor//CrMatcher.h"

class CrASTConsumer : public clang::ASTConsumer {
public:
    CrASTConsumer(clang::Rewriter &rewriter );
    void HandleTranslationUnit(clang::ASTContext &astContext) override {
      mMatchFinder.matchAST(astContext);
    }

private:
    clang::ast_matchers::MatchFinder mMatchFinder;
    CrMatcher mCrMatcher;
    std::string mClassName="Base";
    std::string mOldName="run";;
    std::string mNewName="walk";
};


#endif //CLANG_TUTOR_CRASTCONSUMER_H
