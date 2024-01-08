#!/usr/bin/env bash


/app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/bin/clang++ -v cpp_main_demo.cpp

# -v 给出的具体编译过程如下
'''
clang version 15.0.0 (git@github.com:llvm/llvm-project.git 4ba6a9c9f65bbc8bd06e3652cb20fd4dfc846137)
Target: x86_64-unknown-linux-gnu
Thread model: posix
InstalledDir: /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/bin
Found candidate GCC installation: /usr/lib/gcc-cross/i686-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/11
Found candidate GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12  #注意看这里
#到llvm-project源码中分支【release/15.x】搜索"Selected GCC installation"大概获知 --gcc-toolchain=/usr/lib/gcc/x86_64-linux-gnu/11 可以指定"Selected GCC installation"
#llvm-project源码中分支【release/15.x】 ：  https://gitcode.net/pubz/llvm-project/-/tree/release/15.x
Candidate multilib: .;@m64
Selected multilib: .;@m64
 "/app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/bin/clang-15" -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all --mrelax-relocations -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name cpp_main_demo.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -mllvm -treat-scalable-fixed-error-as-warning -debugger-tuning=gdb -v -fcoverage-compilation-dir=/bal/clang-add-funcIdAsm/test/test_clang++ -resource-dir /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../include/c++ -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/x86_64-linux-gnu -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/backward -internal-isystem /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir=/bal/clang-add-funcIdAsm/test/test_clang++ -ferror-limit 19 -fgnuc-version=4.2.1 -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/cpp_main_demo-c4eb74.o -x c++ cpp_main_demo.cpp
clang -cc1 version 15.0.0 based upon LLVM 15.0.0 default target x86_64-unknown-linux-gnu
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/x86_64-linux-gnu"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/backward"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include"
ignoring nonexistent directory "/include"
#include "..." search starts here:
#include <...> search starts here:
 /usr/lib/gcc/x86_64-linux-gnu/12/../../../../include/c++
 /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0/include
 /usr/local/include
 /usr/include/x86_64-linux-gnu
 /usr/include
End of search list.
'''


#报错如下
'''
cpp_main_demo.cpp:1:10: fatal error: 'iostream' file not found
#include <iostream>
         ^~~~~~~~~~
1 error generated.
'''


