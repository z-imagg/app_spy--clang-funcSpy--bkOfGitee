#!/usr/bin/env bash


source /crk/bochs/bash-simplify/func.sh
source /crk/bochs/bash-simplify/dir_util.sh

getCurScriptDirName $0
#当前脚本文件 绝对路径 CurScriptF, 当前脚本文件 名 CurScriptNm, 当前脚本文件 所在目录 绝对路径 CurScriptNm
#CurScriptDir == /crk/bochs/clang-add-funcIdAsm/
cd $CurScriptDir && \


#llvm15 下载、安装 到目录 /app/llvm_release_home
{ \
LLVM15PkgUrl="https://github.com/llvm/llvm-project/releases/download/llvmorg-15.0.0/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4.tar.xz"
LLVM15Name="clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4"
LLVM15PkgName="$LLVM15Name.tar.xz"
LLVM15PkgMd5F="$LLVM15Name.tar.xz.md5sum.txt"

AppHmD="/app/llvm_release_home"
LLVM15HmD="$AppHmD/$LLVM15Name"

cat << 'EOF' > "${AppHmD}/${LLVM15PkgMd5F}"
24927e91021e97fb07d7c95ee1b4bac5  /app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4.tar.xz
EOF

#0. 创建 AppHome目录 并 进入该目录
function _createAppHomeDirThenEnterIt(){
    sudo mkdir $AppHmD && \
    sudo chown -R z.z $AppHmD && \
    cd $AppHmD
}

ifelse  $CurScriptF $LINENO || true || { \
  test -e $AppHmD && test -d $AppHmD
    "已有AppHomeDir，无需创建"
    cd $AppHmD
  #else:
    _createAppHomeDirThenEnterIt
      "目录$AppHmD 创建完成"
} \
} && \

#1. 下载 LLVM15安装包
{ \
function _downloadLLVM15() {
    wget --output-document=$AppHmD/$LLVM15PkgName  $LLVM15PkgUrl
}

ifelse  $CurScriptF $LINENO || true || { \
  md5sum --check ${AppHmD}/$LLVM15PkgMd5F
    "已有LLVM15包,无需下载"
    :
  #else:
    _downloadLLVM15
      "$LLVM15PkgName 下载完成"
} \
} && \

#2. 解压 LLVM15安装包
{ \
function _unpackLLVM15() {
    rm -fr $LLVM15HmD && \
    echo "解压 $LLVM15PkgName ..." && \
    tar -xf "$LLVM15PkgName" -C .
}

ifelse  $CurScriptF $LINENO || true || { \
  test -f $LLVM15HmD/bin/clang
    "已解压LLVM15安装包,无需解压"
    :
  #else:
    _unpackLLVM15
      "$LLVM15PkgName 解压完成"
} \
} && \

_=end