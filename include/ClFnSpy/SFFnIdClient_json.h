
#ifndef ClFnSpy_FFIdClient_Json_H
#define ClFnSpy_FFIdClient_Json_H
#include <nlohmann/json.hpp>
namespace  web_srv_dto{
//{请求开始
    class FnLctDto{
    public:
        int line;
        int column;
    public:
        FnLctDto(){};
        FnLctDto(int line, int column)
            :
        line(line),
        column(column)
        {}
        //用宏NLOHMANN_DEFINE_TYPE_INTRUSIVE定义方法to_json、from_json，免去手写.
        //  参考: https://github.com/nlohmann/json?tab=readme-ov-file#basic-usage   https://github.com/nlohmann/json?tab=readme-ov-file#examples-1
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(FnLctDto,line,column)
    };
    class FFnIdReq{//请求例子 : { "sF": "user.c ", "fnLct": { "line": 0, "column": 0 } }
    public:
        std::string sF;
        FnLctDto fnLct;
        std::string funcQualifiedName;
    public:
        FFnIdReq(){};
        FFnIdReq(std::string _sF,int line, int column,const std::string funcQualifiedName)
            :
        sF(_sF),
        fnLct(line,column),
        funcQualifiedName(funcQualifiedName)
        {
        };
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(FFnIdReq, sF, fnLct, funcQualifiedName)
    };
//请求结束}

//{响应开始
    class FFnIdRsp{ // 响应例子: { "fId": 2,  "fnAbsLctId": 20001 }
    public:
        int fId;
        int fnAbsLctId;
    public:
        NLOHMANN_DEFINE_TYPE_INTRUSIVE(FFnIdRsp, fId, fnAbsLctId)
    };
//响应结束}

}

#endif
