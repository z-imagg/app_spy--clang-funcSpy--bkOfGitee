#!/usr/bin/env bash


source /bal/bash-simplify/dir_util.sh
getCurScriptDirName $0
#当前脚本文件 绝对路径 CurScriptF, 当前脚本文件 名 CurScriptNm, 当前脚本文件 所在目录 绝对路径 CurScriptNm
#CurScriptDir == /bal/clang-add-funcIdAsm/
cd $CurScriptDir && \

#获取调用者 是否开启了 bash -x  即 是否开启 bash 调试
#返回变量 _out_en_dbg, _out_dbg
get_out_en_dbg && \
#echo "$_out_en_dbg,【$_out_dbg】" && \

PORT=8002 && \
{ alive_pid=$(lsof -t  -i :$PORT) || : ;} && \
{ [ "X$alive_pid" == "X" ] || kill -9 $alive_pid  ;}  && \
sleep 0.2 && \
mvFile_AppendCurAbsTime $CurScriptDir/fId_db.json && \
echo "$CurScriptDir/fId_db.json 已经移动" && \

#miniconda activate
miniconda3Activate && \

#pypi清华源
tsinghua_pypi_src && \

#echo "xxx" && \

pip  install fastapi && \
pip  install "uvicorn[standard]" && \

#启动函数id生成服务
{ nohup python WebMain.py & } && \

sleep 0.5 && \
alive_pid=$(lsof -t  -i :$PORT) && \
echo "启动函数id生成服务(pid:$alive_pid)成功"
