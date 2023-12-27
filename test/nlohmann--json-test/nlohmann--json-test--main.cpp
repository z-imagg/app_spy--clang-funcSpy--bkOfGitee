#include "CTk/SFFnIdClient.h"

#include <iostream>
int main(int Argc, const char **Argv) {

    int srcFileId,fnIdx,funcAbsLocId;
    SFFnIdClient::genFuncAbsLocId("userName.c",12,0,"calcAgeFunc",srcFileId,funcAbsLocId);

    std::cout << "srcFileId:"<< srcFileId << "\n";
    std::cout << "fnIdx:"<< fnIdx << "\n";
    std::cout << "funcAbsLocId:"<< funcAbsLocId << "\n";
    return 0;
}

