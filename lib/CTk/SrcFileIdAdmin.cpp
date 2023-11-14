
#include "CTk/SrcFileIdAdmin.h"
#include <fstream>
#include <filesystem>

/** https://github.com/nlohmann/json  */

void SrcFileIdAdmin::save(){
    std::ofstream ofs(srcFileIdDict_json);
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


json SrcFileIdAdmin::init_load(){
    //当文件srcFileIdDict.json不存在时，写入SrcFileId初始json空字典
    bool fileExisted=std::filesystem::exists(srcFileIdDict_json);
    if(!fileExisted){
        std::ofstream ofs_init(srcFileIdDict_json);
        assert(ofs_init.is_open());
        ofs_init<<"{}";//写入SrcFileId初始json空字典
        ofs_init.close();
    }

    //此时文件srcFileIdDict.json内容一定是json字典
    return json::parse(std::ifstream (srcFileIdDict_json));
}
json SrcFileIdAdmin::srcFileIdDict = SrcFileIdAdmin::init_load();