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

CrASTConsumer::CrASTConsumer(Rewriter &rewriter )
    : mCrMatcher(rewriter)  {
  const auto MatcherForMemberAccess = cxxMemberCallExpr(
      callee(memberExpr(member(hasName(mOldName))).bind("MemberAccess")),
      thisPointerType(cxxRecordDecl(isSameOrDerivedFrom(hasName(mClassName)))));

  mMatchFinder.addMatcher(MatcherForMemberAccess, &mCrMatcher);

  const auto MatcherForMemberDecl = cxxRecordDecl(
      allOf(isSameOrDerivedFrom(hasName(mClassName)),
            hasMethod(decl(namedDecl(hasName(mOldName))).bind("MemberDecl"))));

  mMatchFinder.addMatcher(MatcherForMemberDecl, &mCrMatcher);
}

//-----------------------------------------------------------------------------
// FrontendAction
//-----------------------------------------------------------------------------
class CodeRefactorAddPluginAction : public PluginASTAction {
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
static FrontendPluginRegistry::Add<CodeRefactorAddPluginAction>
    X(/*Name=*/"CodeRefactor",
      /*Desc=*/"Change the name of a class method");
