#include "CodeStyleChecker.h"

void CscASTConsumer::HandleTranslationUnit(clang::ASTContext &Ctx) {
  if (!MainTUOnly)
    Visitor.TraverseDecl(Ctx.getTranslationUnitDecl());
  else {
    // Only visit declarations declared in the input TU
    auto Decls = Ctx.getTranslationUnitDecl()->decls();
    for (auto &Decl : Decls) {
      // Ignore declarations out of the main translation unit.
      //
      // SourceManager::isInMainFile method takes into account locations
      // expansion like macro expansion scenario and checks expansion
      // location instead if spelling location if required.
      if (!SM.isInMainFile(Decl->getLocation()))
        continue;
      Visitor.TraverseDecl(Decl);
    }
  }
}