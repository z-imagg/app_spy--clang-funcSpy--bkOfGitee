
#ifndef CTk_SrcFileIdAdmin_H
#define CTk_SrcFileIdAdmin_H

#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#define srcFileIdDict_json "srcFileIdDict.json"

class SrcFileIdAdmin{
public:
   static int getSrcFileId(std::string srcFilePath);
   static json srcFileIdDict;
   static json init_load();
   static void save();
};

#endif //CTk_SrcFileIdLs_H
