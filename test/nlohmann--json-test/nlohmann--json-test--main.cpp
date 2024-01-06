#include "CTk/SFFnIdClient.h"

#include <iostream>
/* 参考gcc内敛汇编:  https://www.cnblogs.com/sky-heaven/p/7561625.html */
#define FUNC_ID 16
static inline double func01(){
    __asm__  __volatile__ (
            "jmp 0f \n\t"               //0f 表示 0 forward 即 向前跳转到标号0
            "or $0xFFFFFFFF,%%edi \n\t" //$0xFFFFFFFF 表示 常数 0xFFFFFFFF
            "orL %0,%%edi \n\t"         //%0 指代 FUNC_ID 表示 函数id
            "orL $257,%%edi \n\t"       //$257 是 常数257 表示 函数id
            "orL %1,%%edi \n\t"         //%0 指代 func01 表示 函数名
            "0: \n\t"                   //0: 表示 标号0
            :                           //输出 ，无输出
            : "i"(FUNC_ID),"m"(func01)  //输入，i即immedia即立即数, m 即 memory
            );



    return 0.5;
}
int main(int Argc, const char **Argv) {
    func01();

    return 0;
}

