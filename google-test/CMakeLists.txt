#message(STATUS "Begin google-test level CMake file")
project(Processor_Tests)

# 'lib' is the folder with Google Test sources

add_subdirectory(lib/googletest)


include_directories("${gtest_SOURCE_DIR}"/include "${gtest_SOURCE_DIR}")

include_directories(../)
link_directories(../)



# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests


add_executable(Processor_Tests_run test1.cpp)


#If you specify PRIVATE in one, you must specify in both target_link_libraries
target_link_libraries(Processor_Tests_run PRIVATE gtest gtest_main)

target_link_libraries(Processor_Tests_run PRIVATE ProcessorEmulator)

#message(STATUS "End google-test level CMake file")
