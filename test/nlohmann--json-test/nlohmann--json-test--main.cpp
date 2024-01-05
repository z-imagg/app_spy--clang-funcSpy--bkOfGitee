#include "CTk/SFFnIdClient.h"

#include <iostream>
/* 参考gcc内敛汇编:  https://www.cnblogs.com/sky-heaven/p/7561625.html */
#define FUNC_ID 16
static inline double func01(){
    __asm__  __volatile__ (
            "jmp 0f \n\t"
            "or $0xFFFFFFFF,%%edi \n\t"
            "orL %0,%%edi \n\t"
            "orL %1,%%edi \n\t"
            "0: \n\t"
            :
            : "i"(FUNC_ID),"m"(func01)
            );



    return 0.5;
}
int main(int Argc, const char **Argv) {
    func01();

    return 0;
}

