cmake_minimum_required(VERSION 3.0)
project(ToyBox)
set(CMAKE_CXX_STANDARD 17)

# LuaOpener
add_subdirectory(source/LuaOpener)

# JobSystem
add_subdirectory(source/JobSystem)

# test
add_executable(test test/main.cpp test/testJobSystem.h test/testLuaOpener)
target_link_libraries(test PUBLIC lib_JobSystem)
target_include_directories(test PUBLIC source/JobSystem)
target_link_libraries(test PUBLIC lib_LuaOpener)
target_include_directories(test PUBLIC source/LuaOpener)

# copy resource file
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test/test.lua DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )