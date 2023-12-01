#!/bin/bash

#llvm15 下载、安装 到目录 /app/llvm_release_home

LLVM15PkgUrl="https://github.com/llvm/llvm-project/releases/download/llvmorg-15.0.0/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4.tar.xz"
LLVM15Name="clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4"
LLVM15PkgName="$LLVM15Name.tar.xz"
LLVM15PkgMd5F="$LLVM15Name.tar.xz.md5sum.txt"

AppHmD="/app/llvm_release_home"
LLVM15HmD="$AppHmD/$LLVM15Name"


#加载 func.sh中的函数 ifelse
source bash-simplify/func.sh

#当前脚本文件名, 此处 CurScriptF=example.sh
CurScriptF=$(pwd)/$0

#0. 创建 AppHome目录 并 进入该目录
function _createAppHomeDirThenEnterIt(){
    sudo mkdir $AppHmD && \
    sudo chown -R z.z $AppHmD && \
    cd $AppHmD
}

ifelse  $CurScriptF $LINENO
  true || test -e $AppHmD && test -d $AppHmD && \
    true || "已有AppHomeDir，无需创建" && \
    true || cd $AppHmD && \
  #else:
    true || _createAppHomeDirThenEnterIt && \
      true || "目录$AppHmD 创建完成" && \



#1. 下载 LLVM15安装包
function _downloadLLVM15() {
    wget --output-document=$AppHmD/$LLVM15PkgName  $LLVM15PkgUrl
}

ifelse  $CurScriptF $LINENO
  true || md5sum --check $LLVM15PkgMd5F && \
    true || "已有LLVM15包,无需下载" && \
    true || : && \
  #else:
    true || _downloadLLVM15 && \
      true || "$LLVM15PkgName 下载完成" && \


#2. 解压 LLVM15安装包
function _unpackLLVM15() {
    rm -fr $LLVM15HmD && \
    echo "解压 $LLVM15PkgName ..." && \
    tar -xf "$LLVM15PkgName" -C . && \
    md5sum $LLVM15PkgName > $LLVM15PkgMd5F
}

ifelse  $CurScriptF $LINENO
  true || test -f $LLVM15HmD/bin/clang && \
    true || "已解压LLVM15安装包,无需解压" && \
    true || : && \
  #else:
    true || _unpackLLVM15 && \
      true || "$LLVM15PkgName 解压完成"

