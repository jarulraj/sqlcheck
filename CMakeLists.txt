##################################################################################

#Preamble
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project (sqlcheck)

enable_language(C)
enable_language(CXX)

# ---[ C++ Flags
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS         "-Wall -Wno-unknown-pragmas -Wno-sign-compare -Woverloaded-virtual -Wno-write-strings")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

IF(CMAKE_BUILD_TYPE MATCHES COVERAGE)
    message("Coverage mode")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
ENDIF(CMAKE_BUILD_TYPE MATCHES COVERAGE)

# ---[ CTest
include(CTest)

#Environment detection
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Compiler version
# Compiler-specific C++11 activation.
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.9 OR GCC_VERSION VERSION_EQUAL 4.9))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.9 or greater.")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE CLANG_VERSION)
else()
    message(WARNING "You are using an unsupported compiler! Compilation has only been tested with GCC.")
endif ()

# ---[ C++11 Flags
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# -- [ Debug Flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb")

# -- [ GTest
enable_testing()
set(GOOGLETEST_ROOT external/googletest/googletest CACHE STRING "Google Test source root")
include_directories(
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/include
    )
set(GOOGLETEST_SOURCES
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest-all.cc
    ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest_main.cc
    )
foreach(_source ${GOOGLETEST_SOURCES})
    set_source_files_properties(${_source} PROPERTIES GENERATED 1)
endforeach()

add_library(googletest ${GOOGLETEST_SOURCES})

# -- [ GFlags

set(GFLAGS_ROOT external/gflags CACHE STRING "Google Flags")

add_subdirectory(${PROJECT_SOURCE_DIR}/${GFLAGS_ROOT})

include_directories(
    ${PROJECT_BINARY_DIR}/${GFLAGS_ROOT}/include
)

# --[ Threads

find_package(Threads REQUIRED)


# --[ Flags
if(UNIX OR APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -Wextra -Werror -Wno-writable-strings")
endif()

# -- [Info

# Choose the type of build.  Example: SET(CMAKE_BUILD_TYPE Debug) 
MESSAGE( STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE} )
# the compiler used for C++ files 
MESSAGE( STATUS "CMAKE_CXX_COMPILER: " ${CMAKE_CXX_COMPILER} )
# the compiler flags for compiling C++ sources 
MESSAGE( STATUS "CMAKE_CXX_FLAGS: " ${CMAKE_CXX_FLAGS} )


# --[ Packaging

SET(VERSION "1.2")
SET(CPACK_PACKAGE_NAME "sqlcheck")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Automatically identify anti-patterns in SQL queries.")
SET(CPACK_PACKAGE_DESCRIPTION "SQLCheck automates the detection of common anti-patterns in SQL queries. 
Such anti-patterns often increase the time taken to run the queries on a database management system. 
Addressing these anti-patterns will, therefore, help improve the runtime performance of the queries. 
This tool targets all SQL dialects.")
SET(CPACK_PACKAGE_VERSION ${VERSION})
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Joy Arulraj") #required

set(CPACK_PACKAGE_EXECUTABLES "sqlcheck")
set(CPACK_MONOLITHIC_INSTALL TRUE)
if (WIN32)
    # NSIS Setup
    SET(CPACK_GENERATOR "NSIS")
    SET(CPACK_NSIS_MODIFY_PATH TRUE)
    SET(CPACK_NSIS_EXECUTABLES_DIRECTORY "${DISPLAZ_BIN_DIR}")
    SET(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL TRUE)
elseif(APPLE)
    # OSX Bundle generator setup
    SET(CPACK_GENERATOR "DragNDrop")
    SET(CPACK_BUNDLE_NAME "sqlcheck")
elseif(UNIX)
    SET(CPACK_GENERATOR "TGZ;RPM;DEB;ZIP")
endif()
SET(CPACK_STRIP_FILES TRUE)

SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CMAKE_SYSTEM_PROCESSOR}")

INCLUDE(CPack)

# ---[ Subdirectories
add_subdirectory(src)
add_subdirectory(test)

