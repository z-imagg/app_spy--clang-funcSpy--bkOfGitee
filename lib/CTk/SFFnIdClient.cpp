#include "CTk/SFFnIdClient.h"


//#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "cpp_httplib/httplib.h"
#include <string>


#include <nlohmann/json.hpp>
using json = nlohmann::json;

void SFFnIdClient::genFuncAbsLocId(
std::string srcFilePath,int funcDeclBeginPresumedLoc_line,int funcDeclBeginPresumedLoc_column
,
int &srcFileId_output, int &funcAbsLocId_output
        ){
    //TODO 用某 c++ web request lib 访问 本地 SFFnIdService, 解析结果, 并返回结果
    //SFFnId:Source File and Function Id


// HTTP
    httplib::Client client("localhost", 8002);

//    auto response = client.Get("/SrcFileFuncnIdGenService/genFuncAbsLocId");

/**
 * inline Result Client::Post(const std::string &path, const std::string &body,
                           const std::string &content_type)
 */
    std::string url("/SrcFileFuncIdGenService/genFuncAbsLocId");
    std::string body("");//TODO srcFilePath,funcDeclBeginPresumedLoc_line,funcDeclBeginPresumedLoc_column
    std::string content_type("application/json");
    httplib::Result response = client.Post(url, body, content_type);
    response->status;
    std::cout << "响应"<<response->body << "\n";
    nlohmann::basic_json respJson=json::parse(response->body);
    //TODO 还是需要json解析
}