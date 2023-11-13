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


利用  运行clang++的编译 带上本插件.so  实现 对源文件插入时钟滴答语句:
/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/bin/clang++  -Xclang   -load -Xclang /pubx/clang-ctk/cmake-build-debug/lib/libCTk.so  -Xclang   -add-plugin -Xclang  CTk  -c /pubx/clang-ctk/t_clock_tick/test_main.cpp


 */


//std::set<FileID> CTkVst::fileInsertedIncludeStmt;//={};//删除fileInsertedIncludeStmt，不再对间接文件做插入，目前只插入直接文件。
const std::string CTkVst::funcName_TCTk = "X__t_clock_tick";
const std::string CTkVst::IncludeStmt_TCTk = "#include \"t_clock_tick.h\"\n";


static auto _VarDeclAstNodeKind=ASTNodeKind::getFromNodeKind<VarDecl>();

static auto _whileStmtAstNodeKind=ASTNodeKind::getFromNodeKind<WhileStmt>();
static auto _forStmtAstNodeKind=ASTNodeKind::getFromNodeKind<ForStmt>();
static auto _returnStmtAstNodeKind=ASTNodeKind::getFromNodeKind<ReturnStmt>();
static auto _unaryOperatorAstNodeKind=ASTNodeKind::getFromNodeKind<UnaryOperator>();
static auto _implicitCaseExprAstNodeKind=ASTNodeKind::getFromNodeKind<ImplicitCastExpr>();
static auto _CXXStaticCastExprAstNodeKind=ASTNodeKind::getFromNodeKind<CXXStaticCastExpr>();
static auto _ExprAstNodeKind=ASTNodeKind::getFromNodeKind<Expr>();
static auto _CompoundStmtAstNodeKind=ASTNodeKind::getFromNodeKind<CompoundStmt>();












bool CTkVst::insertAfter_X__funcEnter(LocId funcLocId,const char* funcName, SourceLocation funcBodyLBraceLoc ){
  //region 构造插入语句
  std::string cStr_inserted=fmt::format(
          "__asm__  __volatile__ (   \"jmp 0f \\n\\t\"    \"or $0xFFFFFFFF,%%edi \\n\\t\"    \"or ${},%%edi \\n\\t\"    \"0: \\n\\t\" : : ); /*filePath={}, line={}, column={}, funcName={}*/",
          funcLocId.locationId,funcLocId.filePath,funcLocId.line,funcLocId.column,funcName
  );
  llvm::StringRef strRef(cStr_inserted);
  //endregion

  bool insertResult=mRewriter_ptr->InsertTextAfterToken(funcBodyLBraceLoc , strRef);

  //记录已插入语句的节点ID们以防重： 即使重复遍历了 但不会重复插入
  funcEnterLocIdSet.insert(funcLocId);

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
  LocId funcBodyLBraceLocId=LocId::buildFor(filePath, funcBodyLBraceLoc, SM);
  LocId funcBodyRBraceLocId=LocId::buildFor(filePath, funcBodyRBraceLoc, SM);

  //获取返回类型
  const QualType funcReturnType = funcDecl->getReturnType();


  return this->_Traverse_Func(
      funcReturnType,
      false,
      endStmtOfFuncBody,
      funcBodyLBraceLoc,
      funcBodyRBraceLoc,
      funcBodyLBraceLocId,funcBodyRBraceLocId,
      compoundStmt,
      funcQualifiedName.c_str()
      );
}







bool CTkVst::_Traverse_Func(
//  std::function<FuncDesc( )> funcDescGetter,
  QualType funcReturnType,
  bool isaCXXConstructorDecl,
  Stmt *endStmtOfFuncBody,
  SourceLocation funcBodyLBraceLoc,
  SourceLocation funcBodyRBraceLoc,
  LocId funcBodyLBraceLocId,
  LocId funcBodyRBraceLocId,
  CompoundStmt* compoundStmt,
//  bool funcIsConstexpr,
//  bool hasBody,
//  int64_t funcDeclID,
//  Stmt *funcBodyStmt,
  const char* funcName)
{

/////////////////////////对当前节点cxxMethodDecl|functionDecl做 自定义处理


    //region 插入 函数进入语句
      if(Util::LocIdSetNotContains(funcEnterLocIdSet, funcBodyLBraceLocId)){//若没有
//        Util::printStmt(*Ctx, CI, fmt::format("差问题:{:x},",reinterpret_cast<uintptr_t> (&funcEnterLocIdSet)), funcBodyLBraceLocId.to_string(), compoundStmt, true);
        //用funcEnterLocIdSet的尺寸作为LocationId的计数器
          funcBodyLBraceLocId.locationId=funcEnterLocIdSet.size();
        //若 本函数还 没有 插入 函数进入语句，才插入。
        insertAfter_X__funcEnter(funcBodyLBraceLocId,funcName, funcBodyLBraceLoc);
      }
//    }
    //endregion


///////////////////// 自定义处理 完毕


  return false;

}





