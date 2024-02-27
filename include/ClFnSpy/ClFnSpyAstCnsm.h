#ifndef ClFnSpyAstCnsm_H
#define ClFnSpyAstCnsm_H

#include <clang/Rewrite/Core/Rewriter.h>
#include <iostream>
#include <filesystem>
#include <clang/Frontend/CompilerInstance.h>
#include "clang/AST/ASTConsumer.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"

#include <fmt/core.h>

#include "ClFnSpy/ClFnSpyVst.h"
#include "ClFnSpy/CollectIncMacro_PPCb.h"
#include "ClFnSpy/Constant.h"
#include "Util.h"

using namespace llvm;
using namespace clang;

//-----------------------------------------------------------------------------
// ASTConsumer
//-----------------------------------------------------------------------------



class ClFnSpyAstCnsm : public ASTConsumer {
public:
    //Rewriter:3:  Action将Rewriter传递给Consumer
    explicit ClFnSpyAstCnsm(CompilerInstance &_CI, const std::shared_ptr<Rewriter> _rewriter_ptr, ASTContext *_astContext,
                        SourceManager &_SM, LangOptions &_langOptions)
            //Rewriter:4:  Consumer将Rewriter传递给Visitor
            :
            CI(_CI),
            insertVst(_rewriter_ptr, _astContext, _CI, _SM),
            SM(_SM)  {
      //构造函数
//      _rewriter_ptr->overwriteChangedFiles();//C'正常.
    }


    virtual void HandleTranslationUnit(ASTContext &Ctx) override{
      //被上层多次调用 本方法HandleTranslationUnit，后续的调用不再进入实际处理
      if(mainFileProcessed){
        return;
      }

      FileID mainFileId;
      std::string filePath;
      Util::getMainFileIDMainFilePath(SM,mainFileId,filePath);
//      if(Util::endsWith(filePath,".c")){
//        std::cout << fmt::format("忽略c语言源码文件，文件路径:{}\n",filePath)  ;
//        return ;
//      }

      //若是系统文件 或 tick文件则跳过
      if(Util::isSysSrcFile(filePath)  || Util::isTickSrcFile(filePath)){
        return ;
      }

      FrontendOptions &frontendOptions = CI.getFrontendOpts();
      std::cout << "查看，文件路径:" << filePath << ",mainFileId:" << mainFileId.getHashValue() << ",frontendOptions.ProgramAction:" << frontendOptions.ProgramAction << "，Ctx.TUKind:" << Ctx.TUKind <<  std::endl;

      //复制源文件 到 /build/srcCopy/, 开关copySrcFile=true.
      // (适合cmake测试编译器，源文件用完即删除，导致此时出问题后拿不到源文件，难以复现问题）
      if(Util::envVarEq("copySrcFile","true")){
        Util::copySrcFile(filePath,"/build/srcCopy/");
      }


        Constant c;

//////////////////1. 若已插入'#pragma 消息' ，则不用处理
      //时钟函数只插入一次，不重复插入：
      //若已经有时钟函数调用，则标记为已处理，且直接返回，不做任何处理。
      {

      bool hasPragmaMsg = CollectIncMacro_PPCb::pragma_message_set.find(c.NameSpace_funcIdAsmIns) != CollectIncMacro_PPCb::pragma_message_set.end();
      if(hasPragmaMsg){
          //若已经有#include "funcIdBase.h"，则标记为已处理，且直接返回，不做任何处理。
        std::cout << fmt::format("跳过，因为此文件已经被处理, 文件路径:{} 已经包含#pragma消息 {}\n",filePath,c.PrgMsgStmt_funcIdAsmIns) ;
        return;
      }
      }

//////////////////2. 插入时钟语句

      std::cout<<"提示，开始处理编译单元,文件路径:"<<filePath<< ",ClFnSpyAstConsumer:" << this << ",mainFileId:" << mainFileId.getHashValue() << std::endl;

      //暂时 不遍历间接文件， 否则本文件会被插入两份时钟语句
      //{这样能遍历到本源文件间接包含的文件
//      TranslationUnitDecl* translationUnitDecl=Ctx.getTranslationUnitDecl();
//      insertVst.TraverseDecl(translationUnitDecl);
      //}

      //{本循环能遍历到直接在本源文件中的函数定义中
      const DeclContext::decl_range &Decls = Ctx.getTranslationUnitDecl()->decls();
      //const DeclContext::decl_iterator::value_type &declK
      for (clang::Decl* declJ : Decls) {
        if (!SM.isInMainFile(declJ->getLocation())){
          continue;
        }


        insertVst.TraverseDecl(declJ);
      }
      //}
//////////////////3.插入'#pragma 消息'语句

      bool insertResult;
      Util::insertIncludeToFileStart(c.PrgMsgStmt_funcIdAsmIns, mainFileId, SM, insertVst.mRewriter_ptr,insertResult);//此时  insertVst.mRewriter.getRewriteBufferFor(mainFileId)  != NULL， 可以做插入
      std::string msg=fmt::format("插入'#pragma 消息'到文件{},对mainFileId:{},结果:{}\n",filePath,mainFileId.getHashValue(),insertResult);
      std::cout<< msg ;

//////////////////4.应用修改到源文件


      //overwriteChangedFiles引发 "1.	<eof> parser at end of file" 并以  "Program received signal SIGSEGV, Segmentation fault." 退出， 可能原因是修改后的源码有语法错误，侦察错误办法是 overwriteChangedFiles 之前 先调用getRewrittenText获得改后的源码文本，人工查看哪里有语法错误。
//      const std::string &text = insertVst.mRewriter_ptr->getRewrittenText( Ctx.getTranslationUnitDecl()->getSourceRange());
      if(Util::envVarEq("saveTextBefore_overwriteChangedFiles","true")){
        Util::saveRewriteBuffer(insertVst.mRewriter_ptr, mainFileId, filePath + ".getRewriteBufferFor");
        Util::saveEditBuffer(insertVst.mRewriter_ptr, mainFileId, filePath + ".getEditBuffer");
      }

      //CI.getDiagnostics().dump()会输出 本次编译文件、其包含的各个文件 诊断信息.
      if(Util::envVarEq("diagnostics_dump","true")){
        DiagnosticsEngine &diagnostics = CI.getDiagnostics();
        diagnostics.dump();
      }

      insertVst.mRewriter_ptr->overwriteChangedFiles();


      //可以发现, 本方法 两次被调用 ， 对象地址this 即对象ClFnSpyAstCnsm的地址，两次是不同的。 原因在Act中 是 每次都是 新创建 ClFnSpyAstCnsm。
      mainFileProcessed=true;
    }

public:
    CompilerInstance &CI;
    ClFnSpyVst insertVst;
    SourceManager &SM;
    //两次HandleTranslationUnit的ASTConsumer只能每次新建，又期望第二次不要发生，只能让标志字段mainFileProcessed写成static
    static bool mainFileProcessed;
};


#endif