#include "CTk/CTkVst.h"

#include "clang/AST/AST.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "CTk/Util.h"

using namespace clang;

#include <iostream>
#include <clang/AST/ParentMapContext.h>
#include <fmt/core.h>

using namespace llvm;
using namespace clang;
//-----------------------------------------------------------------------------
// CTkVst implementation
//-----------------------------------------------------------------------------
/*


利用  运行clang++的编译 带上本插件.so  实现 对源文件插入funcId生成的嵌入asm语句:
/app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/bin/clang++  -Xclang   -load -Xclang /pubx/analyze_code/clang-ctk/cmake-build-debug/lib/libCTk.so  -Xclang   -add-plugin -Xclang  CTk  -c /pubx/analyze_code/clang-ctk/funcIdBase/test_main.cpp


 */


//std::set<FileID> CTkVst::fileInsertedIncludeStmt;//={};//删除fileInsertedIncludeStmt，不再对间接文件做插入，目前只插入直接文件。


static auto _VarDeclAstNodeKind=ASTNodeKind::getFromNodeKind<VarDecl>();

static auto _whileStmtAstNodeKind=ASTNodeKind::getFromNodeKind<WhileStmt>();
static auto _forStmtAstNodeKind=ASTNodeKind::getFromNodeKind<ForStmt>();
static auto _returnStmtAstNodeKind=ASTNodeKind::getFromNodeKind<ReturnStmt>();
static auto _unaryOperatorAstNodeKind=ASTNodeKind::getFromNodeKind<UnaryOperator>();
static auto _implicitCaseExprAstNodeKind=ASTNodeKind::getFromNodeKind<ImplicitCastExpr>();
static auto _CXXStaticCastExprAstNodeKind=ASTNodeKind::getFromNodeKind<CXXStaticCastExpr>();
static auto _ExprAstNodeKind=ASTNodeKind::getFromNodeKind<Expr>();
static auto _CompoundStmtAstNodeKind=ASTNodeKind::getFromNodeKind<CompoundStmt>();












bool CTkVst::insertAfter_X__funcEnter(bool funcIsStatic,bool funcIsInline,LocId funcLocId, SourceLocation funcBodyLBraceLoc ){
    //用funcEnterLocIdSet的尺寸作为LocationId的计数器
  //region 构造插入语句
const std::string _TypeLs[]={
_TypeX(0),
_TypeX(1) }; const int _TypeCnt=_TypeLs->size();
const std::string InstrLs_FnAddr_InOperand_1st[]={
InstrEmpty_EatInOperand_1st,
InstrStoreFnAddr_InOperand_1st };  const int InstrCnt=InstrLs_FnAddr_InOperand_1st->size();
const std::string InOperandLs_FnAddr__1FmtPlace[]={
Comment_Eat_1FmtPlace,
Immediate_1FmtPlace  };  const int InputOperandCnt=InOperandLs_FnAddr__1FmtPlace->size();
std::string _type;

bool fnStaticOrInline=(funcIsStatic || funcIsInline);
const int type=fnStaticOrInline?InstrTypeEmpty:InstrType1;
assert (type < _TypeCnt);
_type=_TypeLs[type];


std::string instrFnAddr_inOperand_1st= InstrLs_FnAddr_InOperand_1st[type];
std::string inOperand_1FmtPlace= InOperandLs_FnAddr__1FmtPlace[type];
// 若 函数是static或inline则 空注释（即CommentEatLibFmt1） 即 :
//    无法 以 "i"(func)  引用 static函数或inline函数 （若引用，则链接器报错），因此   不插入 "i"(func)
//否则 插入一条 or:
//    "i"(func)  能正常引用 无static 且 无 inline 修饰 的 函数，因此 可以插入 "i"(func)

/* 参考gcc内敛汇编:  https://www.cnblogs.com/sky-heaven/p/7561625.html */
std::string  cStr_inserted=
fmt::format(
"__asm__  __volatile__ ("
"\"jmp 0f \\n\\t\" "     //0f 即 "0 forward" 即 向前跳转到标号0 (备注 向前 即 向下)  。  参考xv6中文件kinit1_func_id__local_label__demo.png
"\"or $0xFFFFFFFF,%%edi \\n\\t\" "
"\"or ${},%%edi \\n\\t\" " //函数id
""+_type+""
""+instrFnAddr_inOperand_1st+""  //第三条指令 容纳 "函数地址"(函数地址相对此指令地址的偏移量) 在指令的操作数中
"\"0: \\n\\t\" "        //标号0 即 内敛汇编紧挨着的原有的c代码
":"     //output operands 即 输出操作数
": "+inOperand_1FmtPlace+"" //input  operands 即 输入操作数
// 不需要 list of clobbered registers
"); /* {} non_static_non_inline_func*/", //注释
funcLocId.abs_location_id, //函数id
funcLocId.funcName,       //输入操作数, 内放 作为 函数地址 的 函数名
funcLocId.to_string()      //注释
  );

  /**这段gcc内敛汇编大约是如下的样子：
__asm__  __volatile__ (
"jmp 0f \n\t"               //0f 表示 0 forward 即 向前跳转到标号0
"or $0xFFFFFFFF,%%edi \n\t" //$0xFFFFFFFF 表示 常数 0xFFFFFFFF
"or $257,%%edi \n\t"        //$257 是 常数257 表示 函数id
"or %0,%%edi \n\t"          //%0 指代 func01 表示 函数名
"0: \n\t"                  //0: 表示 标号0
:                          //输出 ，无输出
: "i"( func01 )           //输入，m 即 memory
);
   */
  llvm::StringRef strRef(cStr_inserted);
  //endregion

  bool insertResult=mRewriter_ptr->InsertTextAfterToken(funcBodyLBraceLoc , strRef);


  //记录已插入语句的节点ID们以防重： 即使重复遍历了 但不会重复插入
  funcEnterLocIdSet.insert(funcLocId);

  //写函数id描述行
//  funcIdDescSrv.write(funcLocId); // 把 funcLocId.to_csv_line() 牵涉的列们 都 发送到 funcIdDescWebService 去

  return insertResult;
}
//TODO 暂时去掉不必要的打印
//TODO 分配变量个数： 当前语句如果是VarDecl














////////////////constexpr

bool CTkVst::TraverseFunctionDecl(FunctionDecl *funcDecl) {
  //跳过非MainFile
  bool _LocFileIDEqMainFileID=Util::LocFileIDEqMainFileID(SM, funcDecl->getLocation());
  if(!_LocFileIDEqMainFileID){
    return false;
  }
  //跳过 default
  if(Util::funcIsDefault(funcDecl)){
    return false;
  }
  //跳过 无函数体
  bool hasBody=funcDecl->hasBody();
  if(!hasBody){
    return false;
  }

  //跳过 constexpr
  bool _isConstexpr = funcDecl->isConstexpr();
  if(_isConstexpr){
    return false;
  }

  //获得 函数体、左右花括号
  Stmt* body  = funcDecl->getBody();
  CompoundStmt* compoundStmt;
  SourceLocation funcBodyLBraceLoc,funcBodyRBraceLoc;
  Util::funcBodyIsCompoundThenGetLRBracLoc(body, compoundStmt, funcBodyLBraceLoc,funcBodyRBraceLoc);

  //跳过 函数体内无语句
  int stmtCntInFuncBody= Util::childrenCntOfCompoundStmt(compoundStmt);
  if(stmtCntInFuncBody<=0){
    return false;
  }

  //获取最后一条语句
  Stmt *endStmtOfFuncBody = Util::endStmtOfCompoundStmt(compoundStmt);

  //获取主文件ID,文件路径
  FileID mainFileId;
  std::string filePath;
  Util::getMainFileIDMainFilePath(SM,mainFileId,filePath);

  //获取函数名称
  const std::string &funcQualifiedName = funcDecl->getQualifiedNameAsString();
//  funcDecl->getNameAsString();
//  funcDecl->getName();

  //按照左右花括号，构建位置id，防止重复插入
  LocId funcBodyLBraceLocId=LocId::buildFor(filePath,funcQualifiedName, funcBodyLBraceLoc, SM);

  //获取返回类型
  const QualType funcReturnType = funcDecl->getReturnType();

  bool funcIsStatic = funcDecl->isStatic();
  bool funcIsInline = Util::funcIsInline(funcDecl);

  return this->_Traverse_Func(//其中的insertAfter_X__funcEnter内Vst.funcEnterLocIdSet、funcLocId.locationId相互配合使得funcLocId.locationId作为funcLocId.srcFileId局部下的自增数
      funcIsStatic,funcIsInline,
      funcReturnType,
      false,
      endStmtOfFuncBody,
      funcBodyLBraceLoc,
      funcBodyLBraceLocId,
      compoundStmt

      );
}







bool CTkVst::_Traverse_Func(
  bool funcIsStatic,
  bool funcIsInline,
  QualType funcReturnType,
  bool isaCXXConstructorDecl,
  Stmt *endStmtOfFuncBody,
  SourceLocation funcBodyLBraceLoc,
  LocId funcBodyLBraceLocId,
  CompoundStmt* compoundStmt
//  bool funcIsConstexpr,
//  bool hasBody,
//  int64_t funcDeclID,
//  Stmt *funcBodyStmt,
  )
{

/////////////////////////对当前节点cxxMethodDecl|functionDecl做 自定义处理


    //region 插入 函数进入语句
      if(Util::LocIdSetNotContains(funcEnterLocIdSet, funcBodyLBraceLocId)){//若没有
//        Util::printStmt(*Ctx, CI, fmt::format("差问题:{:x},",reinterpret_cast<uintptr_t> (&funcEnterLocIdSet)), funcBodyLBraceLocId.to_csv_line(), compoundStmt, true);


        //若 本函数还 没有 插入 函数进入语句，才插入。
        insertAfter_X__funcEnter(funcIsStatic,funcIsInline,funcBodyLBraceLocId, funcBodyLBraceLoc);
      }
//    }
    //endregion


///////////////////// 自定义处理 完毕


  return false;

}





