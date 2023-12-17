#!/usr/bin/env bash


source /crk/bochs/bash-simplify/dir_util.sh
getCurScriptDirName $0
#当前脚本文件 绝对路径 CurScriptF, 当前脚本文件 名 CurScriptNm, 当前脚本文件 所在目录 绝对路径 CurScriptNm
#CurScriptDir == /crk/bochs/clang-add-funcIdAsm/
cd $CurScriptDir && \

export LIB_CTk_CXX_FLAGS="  "
bash _build-release_x.sh
unset LIB_CTk_CXX_FLAGS
#构建的Release版本 :
# -rwxrwxr-x 1 zz zz 874K  7月 24 13:13 /crk/clang-add-funcIdAsm/build/lib/libCTk.so
