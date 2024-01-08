#!/usr/bin/env bash
#先安装g++-12
sudo apt install  g++-12

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
Selected GCC installation: /usr/lib/gcc/x86_64-linux-gnu/12
Candidate multilib: .;@m64
Selected multilib: .;@m64
 "/app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/bin/clang-15" -cc1 -triple x86_64-unknown-linux-gnu -emit-obj -mrelax-all --mrelax-relocations -disable-free -clear-ast-before-backend -disable-llvm-verifier -discard-value-names -main-file-name cpp_main_demo.cpp -mrelocation-model pic -pic-level 2 -pic-is-pie -mframe-pointer=all -fmath-errno -ffp-contract=on -fno-rounding-math -mconstructor-aliases -funwind-tables=2 -target-cpu x86-64 -tune-cpu generic -mllvm -treat-scalable-fixed-error-as-warning -debugger-tuning=gdb -v -fcoverage-compilation-dir=/bal/clang-add-funcIdAsm/test/test_clang++ -resource-dir /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../include/x86_64-linux-gnu/c++/12 -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/backward -internal-isystem /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0/include -internal-isystem /usr/local/include -internal-isystem /usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include -internal-externc-isystem /usr/include/x86_64-linux-gnu -internal-externc-isystem /include -internal-externc-isystem /usr/include -fdeprecated-macro -fdebug-compilation-dir=/bal/clang-add-funcIdAsm/test/test_clang++ -ferror-limit 19 -fgnuc-version=4.2.1 -fcxx-exceptions -fexceptions -fcolor-diagnostics -faddrsig -D__GCC_HAVE_DWARF2_CFI_ASM=1 -o /tmp/cpp_main_demo-453a4f.o -x c++ cpp_main_demo.cpp
clang -cc1 version 15.0.0 based upon LLVM 15.0.0 default target x86_64-unknown-linux-gnu
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include"
ignoring nonexistent directory "/include"
#include "..." search starts here:
#include <...> search starts here:
 /usr/lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12
 /usr/lib/gcc/x86_64-linux-gnu/12/../../../../include/x86_64-linux-gnu/c++/12
 /usr/lib/gcc/x86_64-linux-gnu/12/../../../../include/c++/12/backward  #上一行以此行 是 安装g++-12后 多出来的
 /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/lib/clang/15.0.0/include
 /usr/local/include
 /usr/include/x86_64-linux-gnu
 /usr/include
End of search list.
 "/usr/bin/ld" -pie -z relro --hash-style=gnu --eh-frame-hdr -m elf_x86_64 -dynamic-linker /lib64/ld-linux-x86-64.so.2 -o a.out /lib/x86_64-linux-gnu/Scrt1.o /lib/x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/12/crtbeginS.o -L/app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/bin/../lib/x86_64-unknown-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/12 -L/usr/lib/gcc/x86_64-linux-gnu/12/../../../../lib64 -L/lib/x86_64-linux-gnu -L/lib/../lib64 -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib64 -L/lib -L/usr/lib /tmp/cpp_main_demo-453a4f.o -lstdc++ -lm -lgcc_s -lgcc -lc -lgcc_s -lgcc /usr/lib/gcc/x86_64-linux-gnu/12/crtendS.o /lib/x86_64-linux-gnu/crtn.o

'''

#编译正常


