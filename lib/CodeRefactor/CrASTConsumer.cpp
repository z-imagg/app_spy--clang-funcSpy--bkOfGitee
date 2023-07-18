#include "CodeRefactor.h"

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


CrASTConsumer::CrASTConsumer(Rewriter &rewriter )
        : mCrMatcher(rewriter)  {
  const auto _thisPointerType=thisPointerType(cxxRecordDecl(isSameOrDerivedFrom(hasName(mClassName))));
  const auto _memberExpr=memberExpr(member(hasName(mOldName))).bind("MemberAccess");
  const auto matcherForMemberAccess = cxxMemberCallExpr(
          callee(_memberExpr),
          _thisPointerType);

  mMatchFinder.addMatcher(matcherForMemberAccess, &mCrMatcher);

  const auto _isSameOrDerivedFrom=isSameOrDerivedFrom(hasName(mClassName));
  const auto _decl=decl(namedDecl(hasName(mOldName))).bind("MemberDecl");
  const auto matcherForMemberDecl = cxxRecordDecl(
          allOf(_isSameOrDerivedFrom,
                hasMethod(_decl)));

  mMatchFinder.addMatcher(matcherForMemberDecl, &mCrMatcher);
}