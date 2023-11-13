
set(CTkTest_SOURCES
"${CMAKE_CURRENT_SOURCE_DIR}/../t_clock_tick/t_clock_tick.h"
"${CMAKE_CURRENT_SOURCE_DIR}/../t_clock_tick/test_main.cpp"
)

add_executable(
CTkTest
    ${CTkTest_SOURCES}
)

target_include_directories(
CTkTest
    PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../t_clock_tick"
)