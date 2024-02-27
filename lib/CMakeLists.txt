# THE LIST OF PLUGINS AND THE CORRESPONDING SOURCE FILES
# ======================================================


set(ClFnSpy_SOURCES
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
    add_library(
    ClFnSpy
      SHARED
      ${ClFnSpy_SOURCES}
      )

target_link_libraries(ClFnSpy
        "${LIBFMT_STATIC}"   #/app_spy/fmt//build/libfmt.a
        )

    # Configure include directories for 'plugin'
    target_include_directories(
    ClFnSpy
      PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/../include"
      "${LIBFMT_INCLUDE}"
    )

