
set(CTkTest_SOURCES
"${CMAKE_CURRENT_SOURCE_DIR}/../funcIdBase/funcIdBase.h"
"${CMAKE_CURRENT_SOURCE_DIR}/../funcIdBase/test_main.cpp"
)

add_executable(
CTkTest
    ${CTkTest_SOURCES}
)

target_include_directories(
CTkTest
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../funcIdBase"
)