#!/usr/bin/env bash

#若参数少于2个，则退出（退出码为23）
[ $# -lt 2 ] && exit 4

#echo -n "比较命令：‘diff $1 $2’ , "
echo -n "差异行数："
resultF=/bal/clang-add-funcIdAsm/diffText
diff --side-by-side --suppress-common-lines $1 $2 > $resultF ;
wc -l $resultF
