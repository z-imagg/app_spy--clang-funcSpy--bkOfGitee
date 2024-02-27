
set(ClFnSpyTest_SOURCES
"${CMAKE_CURRENT_SOURCE_DIR}/../funcIdBase/funcIdBase.h"
"${CMAKE_CURRENT_SOURCE_DIR}/../funcIdBase/test_main.cpp"
)

add_executable(
ClFnSpyTest
    ${ClFnSpyTest_SOURCES}
)

target_include_directories(
ClFnSpyTest
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../funcIdBase"
)