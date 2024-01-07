#!/usr/bin/env bash

# me.sh  ${_OutFPathPrefix} ${_prevVarsFIdx} ${gVarsFIdx}
#若参数少于3个，则退出（退出码为55）
[ $# -lt 3 ] && exit 55

_OutFPathPrefix=$1
_prevVarsFIdx=$2
gVarsFIdx=$3
_prevOutFPath="${_OutFPathPrefix}${_prevVarsFIdx}"
outFPath="${_OutFPathPrefix}${gVarsFIdx}"
resultF="${_OutFPathPrefix}diff_${_prevVarsFIdx}_${gVarsFIdx}"

echo -n "比较命令：‘diff ${_prevOutFPath} ${outFPath}’ , "
echo -n "差异行数："
diff --side-by-side --suppress-common-lines ${_prevOutFPath} ${outFPath} > $resultF ;
wc -l $resultF |  perl -pe 'chomp if eof'
#用命令 “ perl -pe 'chomp if eof'  ” 删除wc输出的换行符号
