
#ifndef CTk_SrcFileIdAdmin_H
#define CTk_SrcFileIdAdmin_H

#include <string>

#include <nlohmann/json.hpp>
using json = nlohmann::json;
class SrcFileIdAdmin{
public:
   static int getSrcFileId(std::string srcFilePath);
   static json srcFileIdDict;
   static void save();
};

#endif //CTk_SrcFileIdLs_H
