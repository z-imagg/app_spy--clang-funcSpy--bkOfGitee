#!/usr/bin/env bash

# me.sh  ${_OutFPathPrefix} ${_prevVarsFIdx} ${_curVarsFIdx}
#若参数少于3个，则退出（退出码为55）
[ $# -lt 3 ] && exit 55

_OutFPathPrefix=$1
_prevVarsFIdx=$2
_curVarsFIdx=$3
_prevOutFPath="${_OutFPathPrefix}${_prevVarsFIdx}"
_curOutFPath="${_OutFPathPrefix}${_curVarsFIdx}"
resultF="${_OutFPathPrefix}diff_${_prevVarsFIdx}_${_curVarsFIdx}"

echo -n "比较上次、当前：‘diff ${_prevOutFPath} ${_curOutFPath}’ , "
echo -n "差异行数："
diff --side-by-side --suppress-common-lines ${_prevOutFPath} ${_curOutFPath} > $resultF ;
wc -l $resultF |  perl -pe 'chomp if eof'
#用命令 “ perl -pe 'chomp if eof'  ” 删除wc输出的换行符号
