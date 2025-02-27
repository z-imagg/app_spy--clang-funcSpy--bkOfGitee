#!/usr/bin/env bash

function ifFileNotExistedThenExit(){
#ifFileNotExistedThenExit $filePath $errMsg $exitCode
[ $# -lt 3 ] && exit 11

filePath=$1
errMsg=$2
exitCode=$3

{ \
[ -e ${filePath} ] || \
   {  echo "${errMsg}:【${_prevOutFPath}】，退出码${exitCode}" && exit ${exitCode}  ;} \
;} && return 0

}


# me.sh  ${_OutFPathPrefix} ${_prevVarsFIdx} ${_curVarsFIdx}  $_CMAKE_CURRENT_LIST_FILE0 $_CMAKE_CURRENT_LIST_LINE
#若参数少于5个，则退出（退出码为22）
[ $# -lt 5 ] && exit 22

_OutFPathPrefix=$1
_prevVarsFIdx=$2
_curVarsFIdx=$3
_CMAKE_CURRENT_LIST_FILE0=$4
_CMAKE_CURRENT_LIST_LINE=$5

_CMAKE_CURRENT_LIST_FILE0Base=$(basename $_CMAKE_CURRENT_LIST_FILE0)

_prevOutFPath="${_OutFPathPrefix}${_prevVarsFIdx}"
_curOutFPath="${_OutFPathPrefix}${_curVarsFIdx}"
#差异结果文件名中存放完整的 调用者信息 即 cmake脚本文件名:在该cmake脚本文件的行号
diffResultF="${_OutFPathPrefix}_diff_${_prevVarsFIdx}_${_curVarsFIdx}_Ln_${_CMAKE_CURRENT_LIST_FILE0Base}_${_CMAKE_CURRENT_LIST_LINE}"

ifFileNotExistedThenExit ${_prevOutFPath} "上次输出文件不存在" 33
ifFileNotExistedThenExit ${_curOutFPath} "本次输出文件不存在" 44

#差异结果文件中存放完整的 调用者信息 即 cmake脚本文件路径:在该cmake脚本文件的行号
#echo "#${_CMAKE_CURRENT_LIST_FILE0}:${_CMAKE_CURRENT_LIST_LINE}"> $diffResultF

#完整的差异，不要加'--side-by-side', 否则行被截断
diff  --suppress-common-lines ${_prevOutFPath} ${_curOutFPath} > $diffResultF

echo -n "差异行数："
#'diff --side-by-side'显示的行不完整（一行是被截断的）,但是行数是正确的.
diff --side-by-side --suppress-common-lines ${_prevOutFPath} ${_curOutFPath} | wc -l $diffResultF |  perl -pe 'chomp if eof'
echo -n "，比较上次和当前：‘diff ${_prevOutFPath} ${_curOutFPath}’ "
#用命令 “ perl -pe 'chomp if eof'  ” 删除wc输出的换行符号
