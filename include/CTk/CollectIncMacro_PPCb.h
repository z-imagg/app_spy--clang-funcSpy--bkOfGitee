
#ifndef CollectIncMacro_PPCb_H
#define CollectIncMacro_PPCb_H
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "CTk/LocId.h"


#include <unordered_set>

using namespace clang;

class CollectIncMacro_PPCb : public clang::PPCallbacks {
public:
    CompilerInstance &CI;

    static bool HasIncFuncIdBaseHInited;
    static bool HasIncFuncIdBaseH;

    explicit CollectIncMacro_PPCb(CompilerInstance &_CI) : CI(_CI) {

    }

    //预处理回调收集#includee 以判断case起止范围内 有无#i
    void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok, StringRef FileName, bool IsAngled,
                            CharSourceRange FilenameRange, Optional<FileEntryRef> File, StringRef SearchPath,
                            StringRef RelativePath, const Module *Imported,
                            SrcMgr::CharacteristicKind FileType) override;


    static bool hasInclusionDirective(SourceManager& SM, SourceRange range);
};


#endif //CollectIncMacro_PPCb_H
