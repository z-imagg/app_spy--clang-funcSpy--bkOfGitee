set(Text_gVarsFIdxDesc "变量描述: 'cmake CACHE (INTERNAL)' 即 cmake全局(同级)变量")

if(NOT DEFINED gVarsFIdx)
    set(gVarsFIdx 0 CACHE INTERNAL  ${Text_gVarsFIdxDesc})
    # 'CACHE INTERNAL'变量gVarsFIdx 写入在文件  ./cmake-build-debug/CMakeCache.txt :
    #gVarsFIdx:INTERNAL=28
    message(STATUS "初始化 gVarsFIdx=${gVarsFIdx}")
endif()

function(save_cmake_vars)
#    message(STATUS "函数进入 gVarsFIdx=${gVarsFIdx}")
    MATH(EXPR prevVarsFIdx "${gVarsFIdx} - 1")
    set(prevOutFPath "/tmp/cmake_vars_${prevVarsFIdx}.txt")

    # 获取 cmake变量们
    get_cmake_property(_varNames VARIABLES)

    # cmake变量们写入文件
    set(outFPath "/tmp/cmake_vars_${gVarsFIdx}.txt")
    file(WRITE ${outFPath} "")
    foreach(_varName ${_varNames})
        file(APPEND ${outFPath} "${_varName}=${${_varName}}\n")
    endforeach()

    #文件名下标 自增1
    MATH(EXPR _nextVarsFIdx "${gVarsFIdx} + 1")
    set(gVarsFIdx ${_nextVarsFIdx} CACHE INTERNAL  ${Text_gVarsFIdxDesc})
#####
# 执行 diff 命令
execute_process(
#        COMMAND diff ${outFPath} ${nextOutFPath}
#        COMMAND   bash -x /bal/clang-add-funcIdAsm/diff2f.sh ${prevOutFPath} ${outFPath}
        COMMAND   bash  /bal/clang-add-funcIdAsm/diff2f.sh ${prevOutFPath} ${outFPath}
        RESULT_VARIABLE shExitCode
        OUTPUT_VARIABLE shStdOut
        ERROR_VARIABLE shStdErr
)

# 显示输出
#if(shExitCode EQUAL 0)
#    message(STATUS "文件相同")
#else()
    message(STATUS "退出码：【${shExitCode}】， 标准输出: 【${shStdOut}】， 错误输出:【${shStdErr}】")
#endif()

message(STATUS "函数出来 gVarsFIdx=${gVarsFIdx}")

endfunction()

# 调用函数保存所有变量
#save_cmake_vars()