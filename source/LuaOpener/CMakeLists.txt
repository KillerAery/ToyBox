cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

set ( SRC_CORE LuaValue.h LuaValue.cpp LuaOpener.h LuaOpener.cpp LuaRegister.h )
add_library( lib_LuaOpener STATIC ${SRC_CORE} )

add_subdirectory(${CMAKE_SOURCE_DIR}/source/externals/lua-5.3.6 Lua_DIR)
target_link_libraries(lib_LuaOpener PUBLIC lib_Lua53)
target_include_directories(lib_LuaOpener PUBLIC ${CMAKE_SOURCE_DIR}/source/externals/lua-5.3.6)
