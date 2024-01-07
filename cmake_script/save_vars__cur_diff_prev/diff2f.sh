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


# me.sh  ${_OutFPathPrefix} ${_prevVarsFIdx} ${_curVarsFIdx}
#若参数少于3个，则退出（退出码为22）
[ $# -lt 3 ] && exit 22

_OutFPathPrefix=$1
_prevVarsFIdx=$2
_curVarsFIdx=$3
_prevOutFPath="${_OutFPathPrefix}${_prevVarsFIdx}"
_curOutFPath="${_OutFPathPrefix}${_curVarsFIdx}"
resultF="${_OutFPathPrefix}diff_${_prevVarsFIdx}_${_curVarsFIdx}"

ifFileNotExistedThenExit ${_prevOutFPath} "上次输出文件不存在" 33
ifFileNotExistedThenExit ${_curOutFPath} "本次输出文件不存在" 44

echo -n "比较上次、当前：‘diff ${_prevOutFPath} ${_curOutFPath}’ , "
echo -n "差异行数："
diff --side-by-side --suppress-common-lines ${_prevOutFPath} ${_curOutFPath} > $resultF ;
wc -l $resultF |  perl -pe 'chomp if eof'
#用命令 “ perl -pe 'chomp if eof'  ” 删除wc输出的换行符号
