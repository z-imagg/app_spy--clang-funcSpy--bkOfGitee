#!/bin/bash



#加载 func.sh中的函数 ifelse
# source bash-simplify/func.sh
#PS4='Line ${LINENO}: '    bash -x   ./build-libfmt.sh 
function ifelse(){
    :
}
source bash-simplify/dir_util.sh

#当前脚本文件名, 此处 CurScriptF=example.sh
CurScriptF=$(pwd)/$0


PrjHmD="/crk"


#0. 创建 PrjHome 目录 并 进入该目录

{ \
ifelse  $CurScriptF $LINENO || true || { \
  test -e $PrjHome && test -d $PrjHome
    "已有$PrjHome，无需创建"
    cd $PrjHome 
  #else:
    createDir_CurUsrOwn_EnterIt $PrjHome  
      "目录$PrjHome 创建完成" 
} \
} && \



#1. 下载git仓库 fmt.git

#https://github.com/fmtlib/fmt.git
REPO_URL="https://prgrmz07:ChiPLVkNmksbkDe5pzMF@gitcode.net/pubx/fmtlib/fmt.git"
RepoName="fmt"
REPO_HOME="$PrjHmD/$RepoName"
GitDir="$REPO_HOME/.git/"
Ver="10.0.0"
CmtId="a0b8a92e3d1532361c2f7feb63babc5c18d00ef2"
ErrMsg1="错误, $Ver 的commitId不是 $CmtId, 退出码13"

function _gitClone_() {
git clone   $REPO_URL $REPO_HOME && \
git --git-dir=$GitDir checkout $Ver && \
CurHeadCmtId=$(git --git-dir=$GitDir rev-parse HEAD) && \
{ [ "X$CurHeadCmtId" == "X$CmtId" ]  || { echo $ErrMsg1 && exit 13 ;} ;} && \
_=end

}

{ \
ifelse  $CurScriptF $LINENO || true || { \
  test -e $GitDir && test -d $GitDir
    "已有fmt的git仓库,无需下载"
    :
  #else:
    pwd && _gitClone_
      " $REPO_HOME git仓库 下载完成"
} \
} && \


# set +x
#2. 编译 fmt
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

{ \
ifelse  $CurScriptF $LINENO || true || { \
  test -f $LibFmtArchiv
    "已编译出 $LibFmtArchiv,无需再次编译"
    :
  #else:
    _build_fmt
      "$RepoName 编译完成"
} \
} && \

_=end
########