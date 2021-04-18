cmake_minimum_required(VERSION 3.17)
project(6502Emulator)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES main.cpp Processor.h Memory.h Memory.cpp Processor.cpp)

add_executable(6502Emulator ${SOURCES})

add_subdirectory(google-test)#This used to be lib

##Create object outputs as well as executable
#add_library(Processor STATIC Processor.cpp)
#add_library(Memory STATIC Memory.cpp)