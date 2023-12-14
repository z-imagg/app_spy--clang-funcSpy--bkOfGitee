
#include "CTk/LocId.h"

#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include <fmt/core.h>
#include "CTk/Util.h"

using namespace clang;

    const std::string LocId::csv_field_ls="filePath,line,column,abs_location_id,funcName,srcFileId,locationId";
    LocId LocId::buildFor(std::string fp, const std::string funcQualifiedName, const SourceLocation funcDeclBeginLoc, const clang::SourceManager& SM){

    //  int srcFileId=funcIdDescSrv.getSrcFileId(srcFilePath=fp); // 问funcIdDescSrv要SrcFileId;  SrcFileIdAdmin是funcIdDescSrv的一部分功能
    int srcFileId=0;
      int line;
      int column;
      Util::extractLineAndColumn(SM,funcDeclBeginLoc,line,column);
      return LocId(fp,funcQualifiedName,srcFileId,line,column);
    }

    std::string LocId::to_csv_line(){
        int abs_location_id=this->abs_location_id();
      return fmt::format("{},{},{},{},{},{},{}",filePath,line,column,abs_location_id,funcName,srcFileId,locationId);
    }

    std::string LocId::to_string(){
        int abs_location_id=this->abs_location_id();
        return fmt::format("filePath={},line={},column={},abs_location_id={},funcName={},srcFileId={},locationId={}",
                                filePath,   line,   column,     abs_location_id,    funcName,   srcFileId,  locationId);
    }

LocId:: LocId(
            std::string filePath,const std::string funcQualifiedName, int srcFileId,int line, int column)
    :
      filePath(filePath),
      funcName(funcQualifiedName),
      srcFileId(srcFileId),
      line(line),
    column(column),
    locationId(-1)
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


