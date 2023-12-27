
#ifndef Ctk_FFIdClient_H
#define Ctk_FFIdClient_H

#define HTTP_STATUS_OK 200
//最多重试3次
#define SFFnIdSvr_RETRY_LMT 3

/**访问 源文件Id、函数Id生成服务
 *
 */
#include <string>
class SFFnIdClient{
public:
    static void genFuncAbsLocId(
    std::string srcFilePath,int funcDeclBeginLoc_line,int funcDeclBeginLoc_column, const std::string funcQualifiedName
    ,
    int &srcFileId_output, int& funcIdx_output, int &funcAbsLocId_output
    );
};

#endif
