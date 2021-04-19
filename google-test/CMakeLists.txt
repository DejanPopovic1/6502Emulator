# 'Google_test' is the subproject name
project(Processor_Tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

#ADDED
#include_directories(../)
#add_subdirectory(../)
#END_ADDED

# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
add_executable(Processor_Tests_run test1.cpp)

target_link_libraries(Processor_Tests_run gtest gtest_main)

#target_link_libraries(6502Emulators 6502Emulator)