#ifndef CTkVst_H
#define CTkVst_H


#include <clang/Rewrite/Core/Rewriter.h>
#include <set>
#include <clang/Frontend/CompilerInstance.h>
#include <unordered_set>
#include <fstream>
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
#define _TypeX(x) "\"mov " #x ",%%edi \\n\\t\""
#define InstrTypeEmpty (0)
#define InstrType1 (1)
#define EMPTY_STR ""

//无用 and指令, 其 吃掉第一个 gcc内联汇编输入操作数 即 %0
#define InstrEmpty_EatInOperand_1st "\"and %0,%%edi \\n\\t\" "

//储存函数地址的 or指令, 其 吃掉第一个 gcc内联汇编输入操作数 即 %0
#define InstrStoreFnAddr_InOperand_1st "\"or %0,%%edi \\n\\t\" "

//用注释吃掉一个libfmt::format的位置参数 即 一个 '{}'
#define Comment_Eat_1FmtPlace "\"\"  /* {} */ "

//用注释吃掉一个libfmt::format的位置参数 即 一个 '{}'
#define Immediate_1FmtPlace " \"i\"( {} ) "

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

        //每个clang编译是一个进程，只能首个进程写入csv头，判断有点麻烦，暂时不写csv头
//        ofs_funcIdDescLs<<LocId::csv_field_ls<<"\n";
    }

    ~CTkVst(){
    }


    bool insertAfter_X__funcEnter(bool funcIsStatic,bool funcIsInline,LocId funcLocId,SourceLocation funcBodyLBraceLoc );


    virtual bool TraverseFunctionDecl(FunctionDecl* funcDecl);


    /** 遍历  FunctionDecl 或 CXXMethodDecl
     * TraverseFunctionDecl 和 TraverseCXXMethodDecl 的 公共代码
     * @param funcSourceRange
     * @param funcIsConstexpr
     * @param funcBodyStmt
     * @return
     */
    bool _Traverse_Func(
    bool funcIsStatic,
    bool funcIsInline,
    QualType funcReturnType,
    bool isaCXXConstructorDecl,
    Stmt *endStmtOfFuncBody,
    SourceLocation funcBodyLBraceLoc,
    LocId funcBodyLBraceLocId,
    CompoundStmt* compoundStmt
    );





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





    std::unordered_set<LocId,LocId> funcEnterLocIdSet;



};


#endif