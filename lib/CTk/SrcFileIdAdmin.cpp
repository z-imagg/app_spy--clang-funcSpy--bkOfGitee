
#include "CTk/SrcFileIdAdmin.h"
#include <fstream>

/** https://github.com/nlohmann/json  */

void SrcFileIdAdmin::save(){
    std::ofstream ofs("srcFileIdDict.json");
    ofs << std::setw(4) << srcFileIdDict << std::endl;
}

int SrcFileIdAdmin::getSrcFileId(std::string srcFilePath){
    if(!srcFileIdDict.contains(srcFilePath)){
        srcFileIdDict[srcFilePath]=srcFileIdDict.size();

        //暂时不考虑性能.  合理的逻辑是：SrcFileIdAdmin::save应该放在 真实的退出时机. 但  不知道哪里是 真实的退出时机.  所以 暂时不考虑性能，每次变化时都save。
        SrcFileIdAdmin::save();
    }
    int srcFileId=srcFileIdDict[srcFilePath];
    return srcFileId;
}

json SrcFileIdAdmin::srcFileIdDict = json::parse(std::ifstream ("srcFileIdDict.json"));