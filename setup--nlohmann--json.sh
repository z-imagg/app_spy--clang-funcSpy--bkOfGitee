#!/usr/bin/env bash


source bash-simplify/func.sh

source bash-simplify/dir_util.sh

#当前脚本文件名, 此处 CurScriptF=setup--nlohmann--json.sh
CurScriptF=$(pwd)/$0




#https://github.com/nlohmann/json.git
REPO_HOME="/crk/bochs/clang-add-funcIdAsm/nlohmann--json"
GitDir="$REPO_HOME/"
Ver="develop"
CmtId="a259ecc51e1951e12f757ce17db958e9881e9c6c"

gitCko_tagBrc_asstCmtId $GitDir $Ver $CmtId

#2. 软链接 到 include目录下
inc_dir="/crk/bochs/clang-add-funcIdAsm/include/nlohmann" && \
[ -e $inc_dir ] || ln -s  "$REPO_HOME/include/nlohmann" $inc_dir && \
_=end