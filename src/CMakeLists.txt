##################################################################################

# --[ SQLCheck

# Set output directory
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Create our sqlcheck library
add_library (sqlcheck_library checker.cpp configuration.cpp)

# Make sure the compiler can find include files for our sqlcheck library
target_include_directories (sqlcheck_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Create our executable
add_executable(sqlcheck main.cpp)
target_link_libraries(sqlcheck sqlcheck_library 
${GTEST_BOTH_LIBRARIES} 
${GLOG_LIBRARIES} 
${CMAKE_THREAD_LIBS_INIT}
)