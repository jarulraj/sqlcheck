##################################################################################

## TESTS

# ---[ BASIC TEST
add_executable(basic_test basic_test.cpp)
target_link_libraries(basic_test sqlcheck_library 
${GTEST_BOTH_LIBRARIES} 
googletest
${GLOG_LIBRARIES} 
${CMAKE_THREAD_LIBS_INIT}
)
add_test(NAME BasicTest COMMAND basic_test)

# --[ Add "make check" target

set(CTEST_FLAGS "")
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} ${CTEST_FLAGS} --verbose)

