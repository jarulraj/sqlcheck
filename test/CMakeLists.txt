##################################################################################

## TESTS

# Make sure the compiler can find include files for our sqlcheck library
include_directories (${CMAKE_SOURCE_DIR}/src/include)

# ---[ TEST SUITE
add_executable(test_suite test_suite.cpp)
target_link_libraries(test_suite sqlcheck_library 
${GTEST_BOTH_LIBRARIES} 
googletest
${GLOG_LIBRARIES} 
${CMAKE_THREAD_LIBS_INIT}
)
add_test(NAME TestSuite COMMAND test_suite)

# --[ Add "make check" target

set(CTEST_FLAGS "")
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} ${CTEST_FLAGS} --verbose)

