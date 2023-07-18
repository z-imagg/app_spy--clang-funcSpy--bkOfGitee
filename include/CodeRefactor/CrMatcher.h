
#ifndef CLANG_TUTOR_CRMATCHER_H
#define CLANG_TUTOR_CRMATCHER_H

#include "clang/AST/ASTConsumer.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/FixItRewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"



class CrMatcher
        : public clang::ast_matchers::MatchFinder::MatchCallback {
public:
    explicit CrMatcher(clang::Rewriter &rewriter )
            : mRewriter(rewriter)  {}
    void onEndOfTranslationUnit() override;

    void run(const clang::ast_matchers::MatchFinder::MatchResult &) override;

private:
    clang::Rewriter mRewriter;
    std::string mNewName="walk";
};


#endif //CLANG_TUTOR_CRMATCHER_H
