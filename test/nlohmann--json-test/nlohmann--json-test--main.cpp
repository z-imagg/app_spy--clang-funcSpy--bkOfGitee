
#include <iostream>

#include <fmt/core.h>
/* 参考gcc内敛汇编:  https://www.cnblogs.com/sky-heaven/p/7561625.html */
#define FUNC_ID 257
double func01(){
    int abs_location_id = 99;
    std::string funcLocIdStr="注释";
    std::string cStrDemo=fmt::format(
            "__asm__  __volatile__ ("
            "\"jmp 0f \\n\\t\" "
            "\"or $0xFFFFFFFF,%%edi \\n\\t\" "
            "\"or ${},%%edi \\n\\t\" "
            "\"0: \\n\\t\" "
            ":"
            ":"
            ");"
            "/*{} is_static_or_inline_func*/",
            // "jmp 0f" : 0指后面的标号 "0:", f指的是forward即向前跳(而不是向后跳转)
            //参考xv6中文件kinit1_func_id__local_label__demo.png
            abs_location_id, funcLocIdStr
    );

    std::cout << cStrDemo << std::endl;
//输出: 【__asm__  __volatile__ ("jmp 0f \n\t" "or $0xFFFFFFFF,%%edi \n\t" "or $99,%%edi \n\t" "0: \n\t" ::);/*注释 is_static_or_inline_func*/】


    return 0.5;
}
int main(int Argc, const char **Argv) {
    func01();

    return 0;
}

