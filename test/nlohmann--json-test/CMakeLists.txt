
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1" )
MESSAGE("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
#CMAKE_SOURCE_DIR: /bal/clang-add-funcIdAsm/

MESSAGE("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
#CMAKE_CURRENT_SOURCE_DIR: /bal/clang-add-funcIdAsm/tools

set(nlohmann--json-test_SOURCES
"nlohmann--json-test--main.cpp"
        )

    add_executable(
        nlohmann--json-test
      ${nlohmann--json-test_SOURCES}
      )

    target_link_libraries(
        nlohmann--json-test
        "${LIBFMT_STATIC}"   #/bal/fmt//build/libfmt.a
        )

    target_include_directories(
    nlohmann--json-test
      PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/../../include"
        "${LIBFMT_INCLUDE}"
    )

MESSAGE(STATUS "nlohmann--json-test,,LIBFMT_STATIC:${LIBFMT_STATIC};LIBFMT_INCLUDE:${LIBFMT_INCLUDE}")