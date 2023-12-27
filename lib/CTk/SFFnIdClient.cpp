#include "CTk/SFFnIdClient.h"


//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "cpp_httplib/httplib.h"
#include <string>

#include <fmt/core.h>

#include "CTk/SFFnIdClient_json.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

void SFFnIdClient::genFuncAbsLocId(
std::string srcFilePath, int funcDeclBeginPresumedLoc_line, int funcDeclBeginPresumedLoc_column, const std::string funcQualifiedName
,
int &srcFileId_out,  int &funcAbsLocId_out
        ){
    //SFFnId:Source File and Function Id

    std::string host("localhost");
    int port=8002;
    httplib::Client client(host, port);

    web_srv_dto::FFnIdReq fFnIdReq(srcFilePath,funcDeclBeginPresumedLoc_line,funcDeclBeginPresumedLoc_column,funcQualifiedName);

    //请求例子 : { "sF": "user.c ", "fnLct": { "line": 0, "column": 0 } }

    //请求转json对象
    json req_json_obj = fFnIdReq;

    //json对象转文本
    std::string req_body_text=req_json_obj.dump();

    //接口路径、接口content_type
    std::string api_path("/SrcFileFuncIdGenService/genFuncAbsLocId");
    std::string content_type("application/json");

    //调用接口
    httplib::Result response = client.Post(api_path, req_body_text, content_type);
    auto error=response.error();
    //若第一次连接正常，则不会进入重试循环；否则（即第一次连接失败），则进入重试循环
    {
        int _k=0;
        std::ofstream logw("/crk/error_clang-add-funcIdAsm.log",std::ios_base::app);
        //写错误消息到文件/crk/error_clang-add-funcIdAsm.log
        while(error!=httplib::Error::Success && (_k++) < SFFnIdSvr_RETRY_LMT){//最多重试SFFnIdSvr_RETRY_LMT次
            sleep(1);//等待1秒钟再连接
            std::string errMsg=fmt::format("连接失败, 请检查函数id生成服务是否启动,http://{}:{}/{}\n", host, port, api_path) ;
            logw<<errMsg;
            logw.flush();
            response = client.Post(api_path, req_body_text, content_type);
            error=response.error();
        }

        //如果函数id生成服务不可用，则提出错误消息 并 退出进程
        if(HTTP_STATUS_OK!=response->status){
            //TODO 这个判断理应放在块外，这里是取巧了。 但 只有解决了 clang插件如何在clang编译进程退出时执行自定义代码（关闭日志文件）， 这里才好放在块外。
            int exitCode=41;
            std::string errMsg=fmt::format("404, 请检查函数id生成服务url路径是否正确,http://{}:{}/{},退出码{}\n", host, port, api_path,exitCode) ;
            logw<<errMsg;
            logw.flush();
            logw.close();
            std::cout.flush();
            fflush(stdout);
            std::exit(exitCode);//clang插件中无法正常退出, 打印的消息也不会显示
//            exit(exitCode);
        }

        logw.close();
    }

    // std::cout << "响应状态码"<<response->status << "\n";
    //  响应状态码200
    // std::cout << "响应体"<<response->body << "\n";

    //响应例子: { "fId": 2, "fnIdx": 1, "fnAbsLctId": 20001 }

    //响应文本转json对象
    nlohmann::json rsp_json_obj=json::parse(response->body);

    //json对象转dto 写法1
    web_srv_dto::FFnIdRsp fFnIdRsp=rsp_json_obj;

    //json对象转dto 写法2
//    auto fFnIdRsp=rsp_json_obj.template get<web_srv_dto::FFnIdRsp>();
/*
         int fId;
        int fnIdx;
        int fnAbsLctId;
 */
    //写返回值
    srcFileId_out=fFnIdRsp.fId;
    funcAbsLocId_out=fFnIdRsp.fnAbsLctId;
}