
#ifndef CTk_SrcFileIdAdmin_H
#define CTk_SrcFileIdAdmin_H

#include <string>

class SrcFileIdAdmin{
public:
   static int getSrcFileId(std::string srcFilePath);
private:
    static void load();
    static void save();
};

#endif //CTk_SrcFileIdLs_H
