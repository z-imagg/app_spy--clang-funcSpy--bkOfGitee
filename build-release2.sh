#!/usr/bin/env bash


source /crk/bochs/bash-simplify/dir_util.sh
getCurScriptDirName $0
#当前脚本文件 绝对路径 CurScriptF, 当前脚本文件 名 CurScriptNm, 当前脚本文件 所在目录 绝对路径 CurScriptNm
#CurScriptDir == /crk/bochs/clang-add-funcIdAsm/
cd $CurScriptDir && \

export  LIB_CTk_CXX_FLAGS=" -fno-omit-frame-pointer -Wall   -O0    -g2"
bash _build-release_x.sh
unset LIB_CTk_CXX_FLAGS

#构建的Release版本,带调试信息(变量名称和行号、局部变量),  当出现问题方便排查:
# -rwxrwxr-x 1 zz zz 27M  7月 24 15:03 /crk/clang-add-funcIdAsm/build/lib/libCTk.so
