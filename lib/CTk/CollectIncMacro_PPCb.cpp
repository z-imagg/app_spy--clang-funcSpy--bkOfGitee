#include "CTk/CollectIncMacro_PPCb.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"
#include <clang/Rewrite/Core/Rewriter.h>
#include <iostream>


#include "CTk/Util.h"
#include "CTk/Constant.h"


using namespace clang;

    //预处理回调收集#includee 以判断case起止范围内 有无#i
    void CollectIncMacro_PPCb::InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok, StringRef FileName, bool IsAngled,
                            CharSourceRange FilenameRange, Optional<FileEntryRef> File, StringRef SearchPath,
                            StringRef RelativePath, const Module *Imported,
                            SrcMgr::CharacteristicKind FileType)   {
      //region 方便变量
      SourceManager &SM = CI.getSourceManager();
      //endregion

      //region 获取主文件ID,文件路径
      FileID mainFileId;
      std::string filePath;
      Util::getMainFileIDMainFilePath(SM,mainFileId,filePath);
      //endregion

      //region 跳过非主文件
      if(!SM.isWrittenInMainFile(HashLoc)){
        return;
      }
      //endregion

      Constant c;
      //region 收集 #include指令 位置
        HasIncFuncIdBaseHInited=true;
//        std::cout << "Include指令:" << FileName.str() <<  "," << IncludeTok.getName() << std::endl;//开发打印日志
        if(!HasIncFuncIdBaseH){//防止其他#include xxx.h 破坏已经发现了其实有#include funcIdBase.h的正向结果了
            HasIncFuncIdBaseH = (c.funcIdBaseH == FileName);
        }
      //endregion
    }




bool CollectIncMacro_PPCb::HasIncFuncIdBaseHInited= false;
bool CollectIncMacro_PPCb::HasIncFuncIdBaseH;
