set(_Text_gVarsFIdxDesc "变量描述: 'cmake CACHE (INTERNAL)' 即 cmake全局(同级)变量")
set(_OutFPathPrefix "/tmp/cmake_vars_")

#如果从没有定义过全局变量gVarsFIdx 则定义该全局变量
if(NOT DEFINED gVarsFIdx)
    set(gVarsFIdx 0 CACHE INTERNAL  ${_Text_gVarsFIdxDesc})
    # 'CACHE INTERNAL'变量gVarsFIdx 写入在文件  ./cmake-build-debug/CMakeCache.txt :
    #gVarsFIdx:INTERNAL=数值

    message(STATUS "初始化 gVarsFIdx=${gVarsFIdx}")
endif()

function(save_cmake_vars__cur_diff_prev)
#    message(STATUS "函数进入 gVarsFIdx=${gVarsFIdx}")
    MATH(EXPR _prevVarsFIdx "${gVarsFIdx} - 1")
#    set(_prevOutFPath "${_OutFPathPrefix}${_prevVarsFIdx}")

    set(_curVarsFIdx "${gVarsFIdx}")

    # 获取 cmake变量们
    get_cmake_property(_varNames VARIABLES)

    # cmake变量们写入文件
    set(_curOutFPath "${_OutFPathPrefix}${gVarsFIdx}")
    file(WRITE ${_curOutFPath} "")
    foreach(_varName ${_varNames})
        if (_varName MATCHES "^_"  #忽略_开头的变量名
                OR _varName STREQUAL "gVarsFIdx") #忽略此脚本用到的变量名gVarsFIdx
#            message(STATUS "忽略变量名【${_varName}】")
            continue()
        endif()

        #向文件中写入一行
        file(APPEND ${_curOutFPath} "${_varName}=${${_varName}}\n")
    endforeach()

    #文件名下标 自增1
    MATH(EXPR _nextVarsFIdx "${gVarsFIdx} + 1")
    set(gVarsFIdx ${_nextVarsFIdx} CACHE INTERNAL  ${_Text_gVarsFIdxDesc})
#####
# 执行 diff 命令
execute_process(
#        COMMAND   bash -x  ...
        COMMAND   bash  /bal/clang-add-funcIdAsm/cmake_script/save_vars__cur_diff_prev/diff2f.sh ${_OutFPathPrefix} ${_prevVarsFIdx} ${_curVarsFIdx}
        RESULT_VARIABLE shExitCode
        OUTPUT_VARIABLE shStdOut
        ERROR_VARIABLE shStdErr
)

message(STATUS "保存cmake当前变量们并与上次变量们比较，退出码：【${shExitCode}】， 标准输出: 【${shStdOut}】， 错误输出:【${shStdErr}】")
#message(STATUS "函数出来 gVarsFIdx=${gVarsFIdx}")

endfunction()

# 调用函数保存所有变量
#save_cmake_vars__cur_diff_prev()