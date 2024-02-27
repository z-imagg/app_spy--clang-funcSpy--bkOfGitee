

MESSAGE("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
#CMAKE_SOURCE_DIR: /app_spy/clang-funcSpy/

MESSAGE("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
#CMAKE_CURRENT_SOURCE_DIR: /app_spy/clang-funcSpy/tools

set(ClFnSpyAlone_SOURCES
"${CMAKE_CURRENT_SOURCE_DIR}/../tools/ClFnSpyMain.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/ClFnSpy/Util.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/ClFnSpy/LocId.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/ClFnSpy/FuncDesc.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/ClFnSpy/ClFnSpyVst.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/ClFnSpy/ClFnSpyAstAct.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/ClFnSpy/ClFnSpyAstCnsm.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/ClFnSpy/CollectIncMacro_PPCb.cpp"
"${CMAKE_CURRENT_SOURCE_DIR}/../lib/ClFnSpy/SFFnIdClient.cpp"
        )

    # Create a library corresponding to 'plugin'
    add_executable(
        ClFnSpyAlone
      ${ClFnSpyAlone_SOURCES}
      )

    # Configure include directories for 'tool'
    target_include_directories(
    ClFnSpyAlone
      PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    "${LIBFMT_INCLUDE}"
    )

    # Link in the required libraries
    target_link_libraries(
    ClFnSpyAlone
      "clangTooling"
   "${LIBFMT_STATIC}"  #/bal/fmt//build/libfmt.a
    )
