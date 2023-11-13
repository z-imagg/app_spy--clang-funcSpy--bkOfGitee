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











bool CTkVst::insertBefore_X__tick(LifeStep lifeStep, int64_t stmtId, SourceLocation stmtBeginLoc, int stackVarAllocCnt, int stackVarFreeCnt, int heapObjAllocCnt, int heapObjcFreeCnt, const char* whoInserted){
  //region 构造插入语句
  Util::emptyStrIfNullStr(whoInserted);
  std::string cStr_X__tick;
  cStr_X__tick=fmt::format(
      "{}(/*栈生*/{}, /*栈死*/{}, /*堆生*/{}, /*堆死*/{},&xFuncFrame);/*{}*/",
      CTkVst::funcName_TCTk,
      stackVarAllocCnt,stackVarFreeCnt,heapObjAllocCnt,heapObjcFreeCnt,
      //如果有提供，插入者信息，则放在注释中.
      whoInserted
      );
  //X__t_clock_tick(int stackVarAllocCnt, int stackVarFreeCnt, int heapObjAllocCnt, int heapObjcFreeCnt)
  //"X__t_clock_tick(%d, %d, %d, %d)"
  llvm::StringRef strRef_X__t_clock_tick(cStr_X__tick);
  //endregion

  bool insertResult=mRewriter_ptr->InsertTextBefore(stmtBeginLoc, strRef_X__t_clock_tick);//B.   B处mRewriter和A处mRewriter 地址相同，但A处mRewriter.SourceMgr非空，B处mRewriter为空。

  //记录已插入语句的节点ID们以防重： 即使重复遍历了 但不会重复插入
  if(lifeStep == LifeStep::Alloc){
    allocInsertedNodeIDLs.insert(stmtId);
  }else if(lifeStep == LifeStep::Free){
    freeInsertedNodeIDLs.insert(stmtId);
  }
  return insertResult;
}


bool CTkVst::insertBefore_X__funcReturn(LocId funcBodyRBraceLocId, SourceLocation funcBodyRBraceLoc , const char* whoInserted){
  return CTkVst::insert_X__funcReturn(true, funcBodyRBraceLocId, funcBodyRBraceLoc, whoInserted);
}
bool CTkVst::insertAfter_X__funcReturn( LocId funcBodyRBraceLocId, SourceLocation funEndStmtEndLoc , const char* whoInserted){
  return CTkVst::insert_X__funcReturn(false,funcBodyRBraceLocId,funEndStmtEndLoc,whoInserted);
}
bool CTkVst::insert_X__funcReturn(bool before, LocId funcBodyRBraceLocId, SourceLocation insertLoc , const char* whoInserted){
  //region 构造插入语句
  Util::emptyStrIfNullStr(whoInserted);
  std::string cStr_inserted=fmt::format(
          "/*局部变量xFuncFrame超出作用域后,析构函数自动被调用以完成 函出,无需手工调用X__funcReturn. {}*/",
          //如果有提供，插入者信息，则放在注释中.
          whoInserted
  );
  llvm::StringRef strRef_inserted(cStr_inserted);
  //endregion

  bool insertResult;
  if(before){
    insertResult=mRewriter_ptr->InsertTextBefore(insertLoc, strRef_inserted);
  }else{
    insertResult=mRewriter_ptr->InsertTextAfter(insertLoc, strRef_inserted);
  }

  //记录已插入语句的节点ID们以防重： 即使重复遍历了 但不会重复插入
  funcReturnLocIdSet.insert(funcBodyRBraceLocId);
  return insertResult;
}

bool CTkVst::insertAfter_X__funcEnter(LocId funcLocId,const char* funcName, SourceLocation funcBodyLBraceLoc , const char* whoInserted){
  Util::emptyStrIfNullStr(whoInserted);
  //region 构造插入语句
  std::string cStr_inserted=fmt::format(
          "__asm__  __volatile__ (   \"jmp 0f \\n\\t\"    \"or $0xFFFFFFFF,%%edi \\n\\t\"    \"or ${},%%edi \\n\\t\"    \"0: \\n\\t\" : : ); /*filePath={}, line={}, column={}, funcName={}*/",
          funcLocId.locationId,funcLocId.filePath,funcLocId.line,funcLocId.column,funcName,
          //如果有提供，插入者信息，则放在注释中.
          whoInserted
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








bool CTkVst::TraverseCXXCatchStmt(CXXCatchStmt *cxxCatchStmt) {

/////////////////////////对当前节点cxxCatchStmt做 自定义处理
//  processStmt(cxxCatchStmt,"TraverseCXXCatchStmt");//catch整体 前 肯定不能插入
///////////////////// 自定义处理 完毕

////////////////////  粘接直接子节点到递归链条:  对 当前节点cxxCatchStmt的下一层节点child:{handlerBlock} 调用顶层方法TraverseStmt(child)
  Stmt *handlerBlockStmt = cxxCatchStmt->getHandlerBlock();
  if(handlerBlockStmt){
    Stmt::StmtClass handlerBlockStmtClass = handlerBlockStmt->getStmtClass();
    assert(handlerBlockStmtClass==Stmt::StmtClass::CompoundStmtClass) ;//C++Catch的捕捉体一定是块语句

    TraverseStmt(handlerBlockStmt);
  }
  return true;
}







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
      funcQualifiedName.c_str(),
      "TraverseFunctionDecl",
      "TraverseFunctionDecl:void函数尾非return"
      );
}

bool CTkVst::TraverseCXXConstructorDecl(CXXConstructorDecl* cxxCnstrDecl){
  //跳过非MainFile
  bool _LocFileIDEqMainFileID=Util::LocFileIDEqMainFileID(SM, cxxCnstrDecl->getLocation());
  if(!_LocFileIDEqMainFileID){
    return false;
  }
  //跳过 default
  if(Util::cxxConstructorIsDefault(cxxCnstrDecl)){
    return true;
  }
  //跳过 无函数体
  bool hasBody=cxxCnstrDecl->hasBody();
  if(!hasBody){
    return false;
  }
  //跳过 constexpr
  bool _isConstexpr = cxxCnstrDecl->isConstexpr();
  if(_isConstexpr){
    return false;
  }

  //获得 函数体、左右花括号
  Stmt* body  = cxxCnstrDecl->getBody();
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
  const std::string &funcQualifiedName = cxxCnstrDecl->getQualifiedNameAsString();

  //按照左右花括号，构建位置id，防止重复插入
  LocId funcBodyLBraceLocId=LocId::buildFor(filePath, funcBodyLBraceLoc, SM);
  LocId funcBodyRBraceLocId=LocId::buildFor(filePath, funcBodyRBraceLoc, SM);

  //获取返回类型
  const QualType funcReturnType = cxxCnstrDecl->getReturnType();


  return this->_Traverse_Func(
        funcReturnType,
        true,
        endStmtOfFuncBody,
        funcBodyLBraceLoc,
        funcBodyRBraceLoc,
        funcBodyLBraceLocId,funcBodyRBraceLocId,
        compoundStmt,
        funcQualifiedName.c_str(),
        "TraverseCXXConstructorDecl",
        "TraverseCXXConstructorDecl:构造函数尾非return"
        );
}

bool CTkVst::TraverseCXXMethodDecl(CXXMethodDecl* cxxMethDecl){
  return CTkVst::I__TraverseCXXMethodDecl(cxxMethDecl,"TraverseCXXMethodDecl");
}
bool CTkVst::I__TraverseCXXMethodDecl(CXXMethodDecl* cxxMethDecl,const char* who){
  //跳过非MainFile
  bool _LocFileIDEqMainFileID=Util::LocFileIDEqMainFileID(SM,cxxMethDecl->getLocation());
  if(!_LocFileIDEqMainFileID){
    return false;
  }
  //跳过 default
  if(Util::funcIsDefault(cxxMethDecl)){
    return false;
  }
  //跳过 无函数体
  bool hasBody=cxxMethDecl->hasBody();
  if(!hasBody){
    return false;
  }
  //跳过 constexpr
  bool _isConstexpr = cxxMethDecl->isConstexpr();
  if(_isConstexpr){
    return false;
  }

  //获得 函数体、左右花括号
  Stmt* body = cxxMethDecl->getBody();
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
  const std::string &funcQualifiedName = cxxMethDecl->getQualifiedNameAsString();

  //按照左右花括号，构建位置id，防止重复插入
  LocId funcBodyLBraceLocId=LocId::buildFor(filePath, funcBodyLBraceLoc, SM);
  LocId funcBodyRBraceLocId=LocId::buildFor(filePath, funcBodyRBraceLoc, SM);

  //获取返回类型
  const QualType funcReturnType = cxxMethDecl->getReturnType();

  std::string whoReturn=fmt::format("{}:cpp函数尾非return", who);
  return this->_Traverse_Func(
          funcReturnType,
          false,
          endStmtOfFuncBody,
          funcBodyLBraceLoc,
          funcBodyRBraceLoc,
          funcBodyLBraceLocId, funcBodyRBraceLocId,
          compoundStmt,
          funcQualifiedName.c_str(),
          who,
          whoReturn.c_str()
      );
}


bool CTkVst::TraverseLambdaExpr(LambdaExpr *lambdaExpr) {
  if(sizeof(lambdaExpr)<9999999){//以这样一句话暂时跳过lambda
    return false;
  }

  //跳过非MainFile
  bool _LocFileIDEqMainFileID=Util::LocFileIDEqMainFileID(SM,lambdaExpr->getBeginLoc());
  if(!_LocFileIDEqMainFileID){
    return false;
  }
  //跳过 default
  //跳过 无函数体
  //跳过 constexpr

  //如果一个lambda函数, 捕捉了全部外部变量，则不对其进行任何操作。
  // 因为此时lambda内外都有xFuncFrame变量，感觉有点奇怪。
  for (const auto& capture : lambdaExpr->captures()) {
    if (
//        capture.getCaptureKind() != clang::LCK_This ||
        capture.getCaptureKind() == clang::LCK_ByCopy ||
        capture.getCaptureKind() == clang::LCK_ByRef
    ) {
      return false;
    }
  }

  //获得 函数体、左右花括号
//  Stmt* body  = funcDecl->getBody();
  CompoundStmt* compoundStmt = lambdaExpr->getCompoundStmtBody();
//  CompoundStmt* compoundStmt;
  SourceLocation funcBodyLBraceLoc,funcBodyRBraceLoc;
  Util::GetCompoundLRBracLoc( compoundStmt, funcBodyLBraceLoc,funcBodyRBraceLoc);

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

  //lambda无函数名称
  const char * funName="";

  //按照左右花括号，构建位置id，防止重复插入
  LocId funcBodyLBraceLocId=LocId::buildFor(filePath, funcBodyLBraceLoc, SM);
  LocId funcBodyRBraceLocId=LocId::buildFor(filePath, funcBodyRBraceLoc, SM);

  //获取返回类型
  CXXRecordDecl *cxxRecordDecl = lambdaExpr->getLambdaClass();
  // funcReturnType:
  //        优先lambdaExpr->getLambdaClass()->getLambdaTypeInfo()->getType()，
  //        其次lambdaExpr->getCallOperator()->getReturnType()
  QualType funcReturnType;
  TypeSourceInfo *typeSourceInfo=NULL;
  if(cxxRecordDecl && (typeSourceInfo = cxxRecordDecl->getLambdaTypeInfo()) ){
    funcReturnType=typeSourceInfo->getType();
  }else
  if(CXXMethodDecl *cxxMethodDecl=lambdaExpr->getCallOperator()){
    funcReturnType=cxxMethodDecl->getReturnType();
  }

  //lambda一定有body


  return this->_Traverse_Func(
        funcReturnType,
        false,
        endStmtOfFuncBody,
        funcBodyLBraceLoc,
        funcBodyRBraceLoc,
        funcBodyLBraceLocId,funcBodyRBraceLocId,
        compoundStmt,
        funName,
        "TraverseFunctionDecl",
        "TraverseFunctionDecl:void函数尾非return"
        );
}


bool CTkVst::TraverseCXXConversionDecl(CXXConversionDecl * cxxCnvDecl){
  return CTkVst::I__TraverseCXXMethodDecl(cxxCnvDecl,"TraverseCXXConversionDecl");
}
bool CTkVst::TraverseCXXDestructorDecl(CXXDestructorDecl * cxxDestructorDecl){
  return CTkVst::I__TraverseCXXMethodDecl(cxxDestructorDecl,"CXXDestructorDecl");
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
  const char* funcName,
  const char *whoInsertedFuncEnter,
  const char *whoInsertedFuncReturn)
{

/////////////////////////对当前节点cxxMethodDecl|functionDecl做 自定义处理
  Util::emptyStrIfNullStr(whoInsertedFuncEnter);
  Util::emptyStrIfNullStr(whoInsertedFuncReturn);


    //region 插入 函数进入语句
      if(Util::LocIdSetNotContains(funcEnterLocIdSet, funcBodyLBraceLocId)){//若没有
//        Util::printStmt(*Ctx, CI, fmt::format("差问题:{:x},",reinterpret_cast<uintptr_t> (&funcEnterLocIdSet)), funcBodyLBraceLocId.to_string(), compoundStmt, true);
        //用funcEnterLocIdSet的尺寸作为LocationId的计数器
          funcBodyLBraceLocId.locationId=funcEnterLocIdSet.size();
        //若 本函数还 没有 插入 函数进入语句，才插入。
        insertAfter_X__funcEnter(funcBodyLBraceLocId,funcName, funcBodyLBraceLoc, whoInsertedFuncEnter);
      }
//    }
    //endregion

    //region void函数、构造函数 结尾语句若不是return，则在函数尾 插入 函数释放语句
    if(Util::isVoidFuncOrConstructorThenNoEndReturn(funcReturnType, isaCXXConstructorDecl,endStmtOfFuncBody)){
      insertBefore_X__funcReturn(funcBodyRBraceLocId,funcBodyRBraceLoc,whoInsertedFuncReturn);
    }
    //endregion
//  }

///////////////////// 自定义处理 完毕

////////////////////  粘接直接子节点到递归链条:  对 当前节点cxxMethodDecl|functionDecl的下一层节点child:{body} 调用顶层方法TraverseStmt(child)
    // 粘接直接子节点到递归链条
    if(compoundStmt){
      TraverseStmt(compoundStmt);
    }

  return false;

}





