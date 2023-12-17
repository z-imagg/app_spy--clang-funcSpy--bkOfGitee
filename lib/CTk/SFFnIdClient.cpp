#include "CTk/SFFnIdClient.h"


//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "cpp_httplib/httplib.h"
#include <string>

#include "CTk/SFFnIdClient_json.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

void SFFnIdClient::genFuncAbsLocId(
std::string srcFilePath,int funcDeclBeginPresumedLoc_line,int funcDeclBeginPresumedLoc_column
,
int &srcFileId_output, int &funcAbsLocId_output
        ){
    //SFFnId:Source File and Function Id


    httplib::Client client("localhost", 8002);

    web_srv_dto::FFnIdReq fFnIdReq(srcFilePath,funcDeclBeginPresumedLoc_line,funcDeclBeginPresumedLoc_column);

    //请求例子 : { "sF": "user.c ", "fnLct": { "line": 0, "column": 0 } }

    //请求转json对象
    json req_json_obj = fFnIdReq;

    //json对象转文本
    std::string req_body_text=req_json_obj.dump();

    //接口路径、接口content_type
    std::string url("/SrcFileFuncIdGenService/genFuncAbsLocId");
    std::string content_type("application/json");

    //调用接口
    httplib::Result response = client.Post(url, req_body_text, content_type);

    std::cout << "响应状态码"<<response->status << "\n";
    //  响应状态码200
    std::cout << "响应体"<<response->body << "\n";

    //响应例子: { "fId": 2, "fnIdx": 1, "fnAbsLctId": 20001 }

    //响应文本转json对象
    nlohmann::json rsp_json_obj=json::parse(response->body);

    //json对象转dto 写法1
    web_srv_dto::FFnIdRsp fFnIdRsp=rsp_json_obj;

    //json对象转dto 写法2
//    auto fFnIdRsp=rsp_json_obj.template get<web_srv_dto::FFnIdRsp>();

    //写返回值
    srcFileId_output=fFnIdRsp.fId;
    funcAbsLocId_output=fFnIdRsp.fnAbsLctId;
}