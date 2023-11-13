#ifndef CTkVst_H
#define CTkVst_H


#include <clang/Rewrite/Core/Rewriter.h>
#include <set>
#include <clang/Frontend/CompilerInstance.h>
#include <unordered_set>
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "FuncDesc.h"
#include "LocId.h"

using namespace llvm;
using namespace clang;


//-----------------------------------------------------------------------------
// RecursiveASTVisitor
//-----------------------------------------------------------------------------
class CTkVst
        : public RecursiveASTVisitor<CTkVst> {
public:
public:
    /**
     * 栈变量 或 堆对象 的 生命步骤（生命阶段）
     */
    enum LifeStep{
        Alloc,
        Free
    };
public:
    //Rewriter:4:  Consumer将Rewriter传递给Visitor
    explicit CTkVst(const std::shared_ptr<Rewriter> rewriter_ptr, ASTContext *Ctx, CompilerInstance &CI, SourceManager& SM)
    //Rewriter:5:  Consumer将Rewriter传递给Visitor, 并由Visitor.mRewriter接收
    : mRewriter_ptr(rewriter_ptr),
    Ctx(Ctx),
    CI(CI),
    SM(SM)
    {

    }

    static const std::string funcName_TCTk ;//= "X__t_clock_tick";
    static const std::string IncludeStmt_TCTk ; // = "#include \"t_clock_tick.h\"\n";


    bool insertAfter_X__funcEnter(LocId funcLocId,const char* funcName, SourceLocation funcBodyLBraceLoc , const char* whoInserted);


    /////////constexpr
    virtual bool TraverseFunctionDecl(FunctionDecl* funcDecl);
    virtual bool TraverseCXXMethodDecl(CXXMethodDecl* cxxMethDecl);
    bool I__TraverseCXXMethodDecl(CXXMethodDecl* cxxMethDecl,const char* who);
    virtual bool TraverseCXXConstructorDecl(CXXConstructorDecl* cxxCnstrDecl);
    virtual bool TraverseLambdaExpr(LambdaExpr *lambdaExpr);
    //c++析构函数遍历
    virtual bool TraverseCXXDestructorDecl(CXXDestructorDecl * cxxDestructorDecl);
    //遍历c++类型转换操作符号
    /**
class Person{

};
class User{
public:
    operator Person() const;
};

User::operator Person() const {
    return Person();
}

void f1(){
    User u=User();
    Person p=(Person)u;//类型转换操作符
}
     */
    virtual bool TraverseCXXConversionDecl(CXXConversionDecl * cxxConversionDecl);


    /** 遍历  FunctionDecl 或 CXXMethodDecl
     * TraverseFunctionDecl 和 TraverseCXXMethodDecl 的 公共代码
     * @param funcSourceRange
     * @param funcIsConstexpr
     * @param funcBodyStmt
     * @return
     */
    bool _Traverse_Func(
    QualType funcReturnType,
    bool isaCXXConstructorDecl,
    Stmt *endStmtOfFuncBody,
    SourceLocation funcBodyLBraceLoc,
    SourceLocation funcBodyRBraceLoc,
    LocId funcBodyLBraceLocId,
    LocId funcBodyRBraceLocId,
    CompoundStmt* compoundStmt,
    const char* funcName,
    const char *whoInsertedFuncEnter,
    const char *whoInsertedFuncReturn);


    /** 函数入口  前 插入 检查语句: 检查 上一个返回的 是否 释放栈中其已分配变量 ，如果没 则要打印出错误消息，以方便排查问题。
     *
     * @param funcBody
     * @param functionDeclID
     * @param whoInserted
     */
    void __wrap_insertAfter_X__funcEnter(Stmt *funcBody,int64_t functionDeclID , const char* whoInserted);



public:
    //这里是Visitor中的Rewriter，非源头，不要构造Rewriter，只能引用Act中的源头Rewriter.
    // 若这里也构造，将出现两个Rewriter, 则后一个Rewriter写入时会覆盖前一个Rewriter的写入，导致前一个Rewriter的写入丢失。
    //     这里前一个是命名空间中的函数们，后一个是顶层函数们。
    //     即 看起来的现象： 命名空间中的函数们 时钟调用语句 丢失， 而顶层函数们写入的时中调用语句 还在。
    const std::shared_ptr<Rewriter> mRewriter_ptr;

public:
    ASTContext *Ctx;
    CompilerInstance& CI;
    SourceManager& SM;


    /**其前已经插入语句的 节点ID 们, 为防止重复遍历导致的重复插入，
     * 节点ID集合（防重复插入） 应该按 分配、释放 分开，从而互不干扰
     * 可达到： 即使重复遍历了 但不会重复插入
     * 如果后面发现ID 不是全局唯一的 可以尝试换成 该节点的开始位置
     */
    std::set<int64_t> allocInsertedNodeIDLs;
    std::set<int64_t> freeInsertedNodeIDLs;


    std::unordered_set<LocId,LocId> funcReturnLocIdSet;
    std::unordered_set<LocId,LocId> funcEnterLocIdSet;



};


#endif