set(LLVM_HOME "/app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4")
list(APPEND CMAKE_MODULE_PATH "${LLVM_HOME}")

#list(APPEND CMAKE_PREFIX_PATH "${LLVM_HOME}/lib/cmake/llvm/")
#find_package(LLVM REQUIRED CONFIG)
#此句会 产生变量 LLVM_DIR
message(STATUS "找到 LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "使用 LLVMConfig.cmake in: ${LLVM_DIR}")

list(APPEND CMAKE_PREFIX_PATH "${LLVM_HOME}/lib/cmake/clang/")
find_package(Clang REQUIRED CONFIG)


# We incorporate the CMake features provided by LLVM:
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
include(AddLLVM)

message("LLVM 状态:
  Definitions ${LLVM_DEFINITIONS}
  Includes    ${LLVM_INCLUDE_DIRS}
  Libraries   ${LLVM_LIBRARY_DIRS}
  Targets     ${LLVM_TARGETS_TO_BUILD}"
        )

include_directories(SYSTEM "${LLVM_INCLUDE_DIRS};${CLANG_INCLUDE_DIRS}")
# Now set the LLVM header and library paths:
#include_directories(${LLVM_INCLUDE_DIRS})
#link_directories(${LLVM_LIBRARY_DIRS})
add_definitions(${LLVM_DEFINITIONS})

#set(CMAKE_CXX_STANDARD 17 CACHE STRING "")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall\
#    -fdiagnostics-color=always")
#include(CheckCXXCompilerFlag)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  ")

########
MESSAGE("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")

set(CMAKE_VERBOSE_MAKEFILE ON)


#set(CMAKE_C_COMPILER "${LLVM_HOME}/bin/clang")
set(CMAKE_C_COMPILER "")
set(CMAKE_CXX_COMPILER "${LLVM_HOME}/bin/clang++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
#set(CMAKE_CXX_LINK_EXECUTABLE "${LLVM_HOME}/bin/ld.lld" )#CMAKE_CXX_LINK_EXECUTABLE
#set(CMAKE_LINKER ${LLVM_HOME}/bin/lld)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=${LLVM_HOME}/bin/lld")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fuse-ld=${LLVM_HOME}/bin/ld.lld")
#set(CMAKE_EXE_LINKER_FLAGS "-L${LLVM_HOME}/bin -fuse-ld=ld.lld")

#TODO ： 请解决CMakeLists.txt载入报错（此错误不影响编译、链接）， 看起来 依然在用g++ ，但是已经设置了clang++这不应该了, 估计使其不找g++就能解决了.

#Cannot get compiler information:
#Compiler exited with error code 1: /usr/bin/c++ -xc++ -isystem/app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/include -Wall -fdiagnostics-color=always -fno-rtti -fvisibility-inlines-hidden -fuse-ld=/app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/bin/ld.lld -g -fdiagnostics-color=always -D_GNU_SOURCE -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -std=gnu++17 -fpch-preprocess -v -dD -E
#Using built-in specs.
#COLLECT_GCC=/usr/bin/c++
#OFFLOAD_TARGET_NAMES=nvptx-none:amdgcn-amdhsa
#OFFLOAD_TARGET_DEFAULT=1
#	[01m[Kc++:[m[K [01;31m[Kerror: [m[Kunrecognized command-line option '[01m[K-fuse-ld=/app/llvm_release_home/clang+llvm-15.0.0-x86_64-linux-gnu-rhel-8.4/bin/ld.lld[m[K'
#Target: x86_64-linux-gnu
#Configured with: ../src/configure -v --with-pkgversion='Ubuntu 11.4.0-1ubuntu1~22.04' --with-bugurl=file:///usr/share/doc/gcc-11/README.Bugs --enable-languages=c,ada,c++,go,brig,d,fortran,objc,obj-c++,m2 --prefix=/usr --with-gcc-major-version-only --program-suffix=-11 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --enable-bootstrap --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --enable-default-pie --with-system-zlib --enable-libphobos-checking=release --with-target-system-zlib=auto --enable-objc-gc=auto --enable-multiarch --disable-werror --enable-cet --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none=/build/gcc-11-XeT9lY/gcc-11-11.4.0/debian/tmp-nvptx/usr,amdgcn-amdhsa=/build/gcc-11-XeT9lY/gcc-11-11.4.0/debian/tmp-gcn/usr --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu --with-build-config=bootstrap-lto-lean --enable-link-serialization=2
#Thread model: posix
#Supported LTO compression algorithms: zlib zstd
#gcc version 11.4.0 (Ubuntu 11.4.0-1ubuntu1~22.04)
#[Failed to reload]




MESSAGE("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
#CMAKE_CURRENT_SOURCE_DIR: /bal/clang-add-funcIdAsm/tools

set(nlohmann--json-test_SOURCES
"nlohmann--json-test--main.cpp"
        )


    add_executable(
        nlohmann--json-test
      ${nlohmann--json-test_SOURCES}
      )

target_link_libraries(nlohmann--json-test
        INTERFACE
        ${REQ_LLVM_LIBRARIES}
        )

set_target_properties(nlohmann--json-test PROPERTIES
        LINKER_LANGUAGE CXX
        CXX_STANDARD 17
        )

set_property(TARGET nlohmann--json-test PROPERTY CXX_STANDARD 17)
set_property(TARGET nlohmann--json-test PROPERTY CXX_STANDARD_REQUIRED ON)

    target_link_libraries(
        nlohmann--json-test
        "${LIBFMT_STATIC}"   #/bal/fmt//build/libfmt.a
        )

    target_include_directories(
    nlohmann--json-test
      PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/../../include"
        "${LIBFMT_INCLUDE}"
#            "${LLVM_HOME}/lib/clang/15.0.0/include"
            #-isystem ${LLVM_HOME}/lib/clang/15.0.0/include
    )

#SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
#SET(CMAKE_INSTALL_RPATH "\${ORIGIN}/lib") #指定运行时动态库的加载路径,ORIGIN指运行文件所在目录
#set(CMAKE_INSTALL_RPATH "${LLVM_HOME}/lib/x86_64-unknown-linux-gnu")
#todo: 请解决运行报错： /bal/clang-add-funcIdAsm/cmake-build-debug/bin/nlohmann--json-test: error while loading shared libraries: libc++abi.so.1: cannot open shared object file: No such file or directory

MESSAGE(STATUS "nlohmann--json-test,,LIBFMT_STATIC:${LIBFMT_STATIC};LIBFMT_INCLUDE:${LIBFMT_INCLUDE}")
