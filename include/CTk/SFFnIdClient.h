
#ifndef Ctk_FFIdClient_H
#define Ctk_FFIdClient_H
/**访问 源文件Id、函数Id生成服务
 *
 */
#include <string>
class SFFnIdClient{
public:
    static void genFuncAbsLocId(
    std::string srcFilePath,int funcDeclBeginLoc_line,int funcDeclBeginLoc_column
    ,
    int &srcFileId_output, int &funcAbsLocId_output
    );
};

#endif
