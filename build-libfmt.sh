#!/bin/bash


######{此脚本调试步骤:
###{1. 干运行（置空ifelse）以 确定参数行是否都被短路:
#PS4='[${BASH_SOURCE##*/}] [$FUNCNAME] [$LINENO]: '    bash -x   ./build-libfmt.sh   #bash调试执行 且 显示 行号
#使用 ifelse空函数
# function ifelse(){
#     :
# }
###}


###2. 当 确定参数行都被短路 时, 再 使用 真实 ifelse 函数:
#加载 func.sh中的函数 ifelse
source bash-simplify/func.sh
######}


source bash-simplify/dir_util.sh

#当前脚本文件名, 此处 CurScriptF=build-libfmt.sh
CurScriptF=$(pwd)/$0




#1. 下载git仓库 fmt.git


#https://github.com/fmtlib/fmt.git
REPO_HOME="/crk/bochs/clang-add-funcIdAsm/fmtlib-fmt"
GitDir="$REPO_HOME"
Ver="10.0.0"
CmtId="a0b8a92e3d1532361c2f7feb63babc5c18d00ef2"

gitCko_tagBrc_asstCmtId $REPO_HOME $Ver $CmtId

# set +x
#2. 编译 fmt
{ \
BUILD_HOME="$REPO_HOME/build"
LibFmtArchiv="$BUILD_HOME/libfmt.a"

function _build_fmt() {

_CXX__FLAG=" -fno-omit-frame-pointer -Wall   -O0  -fPIC " && \
echo $BUILD_HOME && rm -fr $BUILD_HOME && mkdir $BUILD_HOME && cd $BUILD_HOME && \
cmake \
-DFMT_TEST=OFF \
-DCMAKE_CXX_FLAGS="$_CXX__FLAG"  -DCMAKE_C_FLAGS="$_CXX__FLAG" \
-S $REPO_HOME && \

make -j8 && \

echo "编译产物如下:" && \
ls -lh $LibFmtArchiv && file $LibFmtArchiv && \

_=end
}

ifelse  $CurScriptF $LINENO || true || { \
  test -f $LibFmtArchiv
    "已编译出 $LibFmtArchiv,无需再次编译"
    :
  #else:
    _build_fmt
      "fmtlib-fm 编译完成"
} \
} && \

_=end
########