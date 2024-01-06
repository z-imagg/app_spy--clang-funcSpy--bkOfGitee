
#include <iostream>
/* 参考gcc内敛汇编:  https://www.cnblogs.com/sky-heaven/p/7561625.html */
#define FUNC_ID 257
static inline double func01(){
    { // 无法 将 当前函数地址 直接 搬到 内敛汇编 中，
   // 即 无法 做到 使得 内敛汇编中 某条指令的操作数  为 当前函数地址
   // 因此 只能让插入的内敛汇编成为一完善体 在 动态执行到 某处时  反映 出 当前函数id、当前函数地址，以下 为 草稿
    void *__funcPtr = (void *) func01;
    __asm__ __volatile__ (
            "jmp 1f \n\t"               //1f 表示 1 forward 即 向前跳转到标号1 （即跳转到 业务代码开始处）

            //{临时变量区域 开始
            "nop \n\t"   "nop \n\t" "nop \n\t" "nop \n\t" //此4字节即 变量var_EAX_origin  用来 保存原eax
            "nop \n\t"   "nop \n\t" "nop \n\t" "nop \n\t" //此4字节即 变量var_EBX_origin  用来 保存原eax
            //临时变量区域 结束}

            //{业务代码 开始

            //{保存业务代码中用到的寄存器们 到 临时变量区域 开始
            "1: \n\t"                   //1: 表示 标号1 业务代码开始处
            "mov EAX,var_EAX_origin \n\t"         //保存原EAX
            "mov EAX,var_EAX_origin \n\t"         //保存原EBX
            //保存业务代码中用到的寄存器们 到 临时变量区域 结束}

            //{业务代码体 开始
            "mov %0,EAX \n\t"         //向EAX写入 当前当前函数id
            "mov %1,EBX \n\t"         //向EBX写入 当前函数相对偏移地址
            "2: \n\t"                   //2: 表示 标号2 业务代码结束处
            //业务代码体 结束}

            //业务代码 结束}

//{本内敛汇编块的识别记号 开始
"jmp 0f \n\t"    //0f 表示 0 forward 即 向前跳转到标号0， 此行及下一行共两条指令 是 bochs 识别 本内敛汇编块的记号. bochs（即bochs源码中的函数logFnEnt） 必须 停在这里 才能正确的拿到 当前当前函数id （即EAX）、 当前函数相对偏移地址（即EBX）
"or $0xFFFFFFFF,%%edi \n\t" //$0xFFFFFFFF 表示 常数 0xFFFFFFFF
//本内敛汇编块的识别记号 结束}

            //{从 临时变量区域 恢复 业务代码中用到的寄存器们 开始
            "0: \n\t"                   //0: 表示 标号0
            "mov var_EAX_origin,EAX \n\t"         //恢复原EAX
            "mov var_EAX_origin,EAX \n\t"         //恢复原EBX
            //从 临时变量区域 恢复 业务代码中用到的寄存器们 结束}

            :                           //输出 ，无输出
            : "i"(FUNC_ID), "m"(__funcPtr)  //输入，i即immedia即立即数, m即memory
            );
    }



    return 0.5;
}
int main(int Argc, const char **Argv) {
    func01();

    return 0;
}

