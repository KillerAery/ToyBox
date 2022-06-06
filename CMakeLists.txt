cmake_minimum_required(VERSION 3.0)
project(ToyBox)
set(CMAKE_CXX_STANDARD 17)

# LuaOpener
add_subdirectory(source/externals/lua-5.3.6)
add_library(lib_LuaOpener STATIC source/LuaOpener/LuaOpener.cpp)
target_link_libraries(lib_LuaOpener PUBLIC lib_Lua53)
target_include_directories(lib_LuaOpener PUBLIC source/externals/lua-5.3.6)

# JobSystem
add_library(lib_JobSystem STATIC source/JobSystem/JobSystem.cpp)

# test
add_executable(test test/main.cpp)
target_link_libraries(test PUBLIC lib_LuaOpener)
target_include_directories(test PUBLIC source/LuaOpener)
target_link_libraries(test PUBLIC lib_JobSystem)
target_include_directories(test PUBLIC source/JobSystem)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(CMAKE_DEBUG_POSTFIX "_d")
set(CMAKE_RELEASE_POSTFIX "_r") 
set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "_d")
set_target_properties(${TARGET_NAME} PROPERTIES RELEASE_POSTFIX "_r")