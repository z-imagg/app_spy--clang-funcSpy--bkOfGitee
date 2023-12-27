
#include "CTk/LocId.h"
#include "CTk/SFFnIdClient.h"

#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include <fmt/core.h>
#include "CTk/Util.h"

using namespace clang;

    LocId LocId::buildFor(std::string srcFilePath, const std::string funcQualifiedName, const SourceLocation funcDeclBeginLoc, const clang::SourceManager& SM){
   clang::PresumedLoc fnDeclBgPresumedLoc = SM.getPresumedLoc(funcDeclBeginLoc);
    int fnDclBgPrsmLc_line=fnDeclBgPresumedLoc.getLine();
    int fnDclBgPrsmLc_column=fnDeclBgPresumedLoc.getColumn();
 int srcFileId;
// int funcIdx;
 int abs_location_id;
 LocId funcLocId(srcFilePath, funcQualifiedName,  fnDclBgPrsmLc_line, fnDclBgPrsmLc_column);
  SFFnIdClient::genFuncAbsLocId(
          srcFilePath, fnDclBgPrsmLc_line, fnDclBgPrsmLc_column, funcQualifiedName
,
          funcLocId.srcFileId, funcLocId.abs_location_id
         );

      return funcLocId;
    }

    std::string LocId::to_csv_line(){
      return fmt::format("{},{},{},{},{},{}",filePath,line,column,abs_location_id,funcName,srcFileId);
    }

    std::string LocId::to_string(){
        return fmt::format("filePath={},line={},column={},abs_location_id={},funcName={},srcFileId={}",
                                filePath,   line,   column,     abs_location_id,    funcName,   srcFileId);
    }

LocId:: LocId(
            std::string filePath,const std::string funcQualifiedName,  int line, int column)
    :
      filePath(filePath),
      funcName(funcQualifiedName),
      line(line),
    column(column)
    {

    }



    // 重写哈希函数
    size_t LocId::operator()(const LocId& that) const {
      // 同一个文件的 在同一个链条（hash）
      return std::hash<std::string>()(that.filePath) ;
    }

    // 重写相等操作符
    bool LocId::operator==(const LocId& that) const {
      return
//      this->stmtClass== that.stmtClass &&
      this->line== that.line
       && this->column== that.column
       && this->filePath== that.filePath
      ;
    }


