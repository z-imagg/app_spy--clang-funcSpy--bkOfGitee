
#ifndef LocId_H
#define LocId_H


#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include <fmt/core.h>
//这里如果include Util.h, 而Util.h又inlucde了LocId.h, 即 交叉include 。
//  这样编译器没法编译，解决办法是 将LocId.h中 用到了Util.h的函数移动到实现文件LocId.cpp中
using namespace clang;


/**
std::set : 排序, 去重

std::unordered_set : 无排序,  去重。  这是这里的选择
 */

class LocId{
public:
    const static std::string csv_field_ls;
    static LocId buildFor(std::string srcFilePath, const std::string funcQualifiedName, const SourceLocation funcDeclBeginLoc, const clang::SourceManager& SM);

    std::string to_csv_line();
    std::string to_string();
public:
    std::string filePath;
    int srcFileId;//srcFileId==fId

    int line;
    int column;
    int abs_location_id;

    //函数名 ： 冗余字段
    const std::string funcName;

    LocId( ){};
    LocId(
//            Decl::Kind declKind, Stmt::StmtClass stmtClass,
            std::string filePath,const std::string funcQualifiedName, int line, int column);

    // 重写哈希函数
    size_t operator()(const LocId& that) const ;

    // 重写相等操作符
    bool operator==(const LocId& that) const ;



};
#endif //LocId_H
