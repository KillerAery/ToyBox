cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)

set ( SRC_CORE Job.h LockfreeQueue.h ThreadPool.h JobSystem.h JobSystem.cpp)
add_library( lib_JobSystem STATIC ${SRC_CORE} )