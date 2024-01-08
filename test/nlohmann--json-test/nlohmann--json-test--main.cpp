
/* 参考gcc内敛汇编:  https://www.cnblogs.com/sky-heaven/p/7561625.html */
#define FUNC_ID 257

//inline double func01(){
//   若 inline 函数， 则 链接器 报错:  ld.lld: error: cannot preempt symbol: func01()  .  preempt: 取代

//static double func01(){
//   若 static 函数， 则 链接器 报错: ld.lld: error: relocation R_X86_64_32 cannot be used against local symbol; recompile with -fPIC .  注意报错信息 中的 local(本地) 即 static

double func01(){
//   若 无static 无inline 的函数, 则 正常链接
    __asm__  __volatile__ (
            "jmp 0f \n\t"               //0f 表示 0 forward 即 向前跳转到标号0
            "or $0xFFFFFFFF,%%edi \n\t" //$0xFFFFFFFF 表示 常数 0xFFFFFFFF
            "orL %0,%%edi \n\t"         //%0 指代 FUNC_ID 表示 函数id
            "orL %1,%%edi \n\t"         //%0 指代 func01 表示 函数名
            "0: \n\t"                   //0: 表示 标号0
            :                           //输出 ，无输出
//            : "i"(FUNC_ID),"i"(222)  //输入，i即immedia即立即数,"i"(222)当然是能正常编译链接
            : "i"(FUNC_ID),"i"(func01)  //输入，i即immedia即立即数,  static(本地)函数或inline函数中有"i"(func01)则链接失败，而一般函数内有"i"(func01)正常链接
            );
//编译、链接正常


    return 0.5;
}
int main(int Argc, const char **Argv) {
    func01();

    return 0;
}

