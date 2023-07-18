
#ifndef CLANG_TUTOR_CSCASTCONSUMER_H
#define CLANG_TUTOR_CSCASTCONSUMER_H

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"

#include "CodeStyleChecker/CscVisitor.h"
class CscASTConsumer : public clang::ASTConsumer {
public:
    explicit CscASTConsumer(clang::ASTContext *Context,
                            bool MainFileOnly,
                            clang::SourceManager &SM)
            : Visitor(Context), SM(SM), MainTUOnly(MainFileOnly) {}

    void HandleTranslationUnit(clang::ASTContext &Ctx) ;

private:
    CscVisitor Visitor;
    clang::SourceManager &SM;
    // Should this plugin be only run on the main translation unit?
    bool MainTUOnly = true;
};

#endif //CLANG_TUTOR_CSCASTCONSUMER_H
