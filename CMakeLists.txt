#message(STATUS "Begin root level CMake file")
cmake_minimum_required(VERSION 3.17)
project(6502Emulator)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES Processor.h Memory.h Memory.cpp Processor.cpp testingFile.cpp testingFile.h)#REMOVED MAIN

add_subdirectory(google-test)#IF THIS IS MOVED TO THE END, THEN WE CANT RUN THE TEST ENTRY POINT


add_library(ProcessorEmulator STATIC "${SOURCES}")

#add_executable(6502Emulator ${SOURCES})#COMMENTED THIS OUT

#message(STATUS "End root level CMake file")


##Create object outputs as well as executable
#add_library(Processor STATIC Processor.cpp)
#add_library(Memory STATIC Memory.cpp)