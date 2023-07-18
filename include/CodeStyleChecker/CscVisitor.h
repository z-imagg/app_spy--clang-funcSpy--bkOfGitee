
#ifndef CLANG_TUTOR_CSCVISITOR_H
#define CLANG_TUTOR_CSCVISITOR_H

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"


class CscVisitor
        : public clang::RecursiveASTVisitor<CscVisitor> {
public:
    explicit CscVisitor(clang::ASTContext *Ctx) : Ctx(Ctx) {}
    bool VisitCXXRecordDecl(clang::CXXRecordDecl *Decl);
    bool VisitFunctionDecl(clang::FunctionDecl *Decl);
    bool VisitVarDecl(clang::VarDecl *Decl);
    bool VisitFieldDecl(clang::FieldDecl *Decl);

private:
    clang::ASTContext *Ctx;

    // Checks whether the name in Decl contains any `_`. Issues a warnning if it
    // does.
    void checkNoUnderscoreInName(clang::NamedDecl *Decl);
    // Checks whether the name in Decl starts with a lower case letter. Issues a
    // warning if not.
    void checkNameStartsWithLowerCase(clang::NamedDecl *Decl);
    // Checks whether the name in Decl starts with an upper case letter. Issues a
    // warning if not.
    void checkNameStartsWithUpperCase(clang::NamedDecl *Decl);
};


#endif //CLANG_TUTOR_CSCVISITOR_H
