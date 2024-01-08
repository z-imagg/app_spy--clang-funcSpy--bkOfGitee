/**
/bal/clang-add-funcIdAsm/cmake-build-debug/bin/CTkAlone /bal/clang-add-funcIdAsm/funcIdBase/test_main.cpp
clang插件输出日志如下，正确识别了 static函数
  开发查问题日志funcIsStatic_funcIsInline:【/bal/clang-add-funcIdAsm/funcIdBase/test_main.cpp:2:1;funcQualifiedName】,funcIsStatic=true,funcIsInline=false
 */
static int zlib_inflateInit2(int strm, int windowBits)
{
    return 0;
}