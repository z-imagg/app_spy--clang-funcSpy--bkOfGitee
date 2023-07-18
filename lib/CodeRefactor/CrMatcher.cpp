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

void CrMatcher::run(const MatchFinder::MatchResult &matchResult) {
  const MemberExpr *memberAccess =
          matchResult.Nodes.getNodeAs<clang::MemberExpr>("MemberAccess");

  if (memberAccess) {
    SourceRange callExprSrcRange = memberAccess->getMemberLoc();
    mRewriter.ReplaceText(callExprSrcRange, mNewName);
  }

  const NamedDecl *memberDecl =
          matchResult.Nodes.getNodeAs<clang::NamedDecl>("MemberDecl");

  if (memberDecl) {
    SourceRange memberDeclSrcRange = memberDecl->getLocation();
    mRewriter.ReplaceText(
            CharSourceRange::getTokenRange(memberDeclSrcRange), mNewName);
  }
}

void CrMatcher::onEndOfTranslationUnit() {
  // Output to stdout
  mRewriter
          .getEditBuffer(mRewriter.getSourceMgr().getMainFileID())
          .write(llvm::outs());
}