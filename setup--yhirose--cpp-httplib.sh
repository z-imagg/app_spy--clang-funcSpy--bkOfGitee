#!/usr/bin/env bash


source bash-simplify/func.sh

source bash-simplify/dir_util.sh

#当前脚本文件名, 此处 CurScriptF=setup--yhirose--cpp-httplib.sh
CurScriptF=$(pwd)/$0





#1. 下载git仓库 yhirose/cpp-httplib.git


#https://github.com/yhirose/cpp-httplib.git
REPO_HOME="/crk/bochs/clang-add-funcIdAsm/yhirose--cpp-httplib"
GitDir="$REPO_HOME/"
Ver="v0.14.2"
CmtId="f14accb7b6ff4499321e14c61497bc7e4b28e49b"

gitCko_tagBrc_asstCmtId $GitDir $Ver $CmtId

#2. 软链接 到 include目录下
inc_dir="/crk/bochs/clang-add-funcIdAsm/include/cpp_httplib" && \
[ -e $inc_dir ] || ln -s  $REPO_HOME $inc_dir && \
_=end