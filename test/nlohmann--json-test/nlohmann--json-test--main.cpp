#include "CTk/SFFnIdClient.h"

#include <iostream>
/* 参考gcc内敛汇编:  https://www.cnblogs.com/sky-heaven/p/7561625.html */
double func01(char ch, float flt){
    int integer=ch*flt;
    __asm__  __volatile__ (
            "jmp 0f \n\t"
            "or $0xFFFFFFFF,%%edi \n\t"
            "or $11,%%edi \n\t"
            "or %0,%%edi \n\t"
            "0: \n\t"
            :
            : "m"(func01)
            );
    double  result=integer/ch+flt;
    return result;
}
int main(int Argc, const char **Argv) {
    func01('A', 999.0);

    return 0;
}

