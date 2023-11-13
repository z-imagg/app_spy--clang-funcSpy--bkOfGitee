
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
    static LocId buildFor(std::string fp, const SourceLocation funcDeclBeginLoc, const clang::SourceManager& SM);

    std::string to_csv_line();
    std::string to_string();
public:
    std::string filePath;
    int srcFileId;

    int line;
    int column;
    /* locationId 与 <filePath,line,column> 一一对应 */
    int locationId;

    //函数名 ： 冗余字段
    const char* funcName;

    LocId( ){};
    LocId(
//            Decl::Kind declKind, Stmt::StmtClass stmtClass,
            std::string filePath,int srcFileId, int line, int column);


    // 重写哈希函数
    size_t operator()(const LocId& that) const ;

    // 重写相等操作符
    bool operator==(const LocId& that) const ;


    const int LIMIT_FUNC_IN_1_SRC_FILE=10000;
    /**
     * 一个源文件中最大支持 LIMIT_FUNC_IN_1_SRC_FILE(即10000) 个函数
     * 如果超出界限，则占据到下一个源文件的funcId范围了，显然是严重错误
     * 指令中以四字节存储的funcId, 因此最多源文件数目是 2**32/(LIMIT_FUNC_IN_1_SRC_FILE 即10**4) == 429496.7296
     * @return
     */
    int abs_location_id(){
        //一个源文件中最大支持 10000 个函数

        //如果超出界限，则占据到下一个源文件的funcId范围了，显然是严重错误
        assert(locationId<LIMIT_FUNC_IN_1_SRC_FILE);

        return srcFileId*(LIMIT_FUNC_IN_1_SRC_FILE)+locationId;
    }

};
#endif //LocId_H
