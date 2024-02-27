
#用法 include("/app_spy/clang-funcSpy/libfmt.cmake")

set(LIBFMT_DIR "/app_spy/fmtlib-fmt/")
#set(LIBFMT_STATIC /app_spy/fmt//include)
set(LIBFMT_INCLUDE "${LIBFMT_DIR}/include/")
#set(LIBFMT_STATIC /app_spy/fmt//build/libfmt.a)
set(LIBFMT_STATIC "${LIBFMT_DIR}/build/libfmt.a")

if (NOT EXISTS "${LIBFMT_STATIC}")
  MESSAGE(FATAL_ERROR "libfmt静态库${LIBFMT_STATIC} 不存在，请参照 build-libfmt.sh 构建libfmt静态库")
endif()

if (NOT EXISTS "${LIBFMT_INCLUDE}")
  MESSAGE(FATAL_ERROR "libfmt头文件目录${LIBFMT_INCLUDE} 不存在，请参照 build-libfmt.sh 构建libfmt静态库")
endif()

MESSAGE(STATUS "LIBFMT_STATIC:${LIBFMT_STATIC};LIBFMT_INCLUDE:${LIBFMT_INCLUDE}")
