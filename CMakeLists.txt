##################################################################################

#Preamble
cmake_minimum_required (VERSION 2.6)
project (machine C CXX)

# ---[ CTest
include(CTest)

#Environment detection
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# ---[ C++11 Flags
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++1y" COMPILER_SUPPORTS_CXX1Y)

if(COMPILER_SUPPORTS_CXX1Y)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++1Y support. Please use a different C++ compiler.")
endif()

# -- [ Debug Flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb")

# -- [ GTest
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# -- [ Threads

find_package(Threads REQUIRED)

# -- [ Glog

find_package(Glog REQUIRED)
include_directories(${GLOG_INCLUDE_DIRS})

# -- [ Gflags

find_package(GFlags REQUIRED)

# ---[ Flags
if(UNIX OR APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -Wextra -Werror -lpthread")
endif()

# ---[ Subdirectories
add_subdirectory(src)
add_subdirectory(test)