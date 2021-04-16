cmake_minimum_required(VERSION 3.17)
project(6502Emulator)

set(CMAKE_CXX_STANDARD 14)

add_executable(6502Emulator main.cpp main.h Processor.h Memory.h Memory.cpp Processor.cpp)

#add_library(Processor STATIC Processor.cpp)
#add_library(Memory STATIC Memory.cpp)

add_subdirectory(google-test)#This used to be lib