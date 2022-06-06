cmake_minimum_required ( VERSION 2.8 )

include_directories ( ${CMAKE_CURRENT_BINARY_DIR} )
set ( SRC_CORE lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c lfunc.c lgc.c llex.c
  lmem.c lobject.c lopcodes.c lparser.c lstate.c lstring.c ltable.c
  ltm.c lundump.c lvm.c lzio.c )
set ( SRC_LIB lauxlib.c lbaselib.c lbitlib.c lcorolib.c ldblib.c liolib.c
  lmathlib.c loslib.c lstrlib.c ltablib.c lutf8lib.c loadlib.c linit.c )

add_library ( lib_Lua53 ${SRC_CORE} ${SRC_LIB} )
set_target_properties ( lib_Lua53 PROPERTIES OUTPUT_NAME Lua53 )

# 目前不需要 dll
# add_library ( lib_Lua53_dll SHARED ${SRC_CORE} ${SRC_LIB} )
# target_compile_definitions ( lib_Lua53_dll PRIVATE _CRT_SECURE_NO_WARNINGS LUA_BUILD_AS_DLL )
# set_target_properties ( lib_Lua53_dll PROPERTIES OUTPUT_NAME Lua53 )