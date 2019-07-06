cmake_minimum_required(VERSION 3.9)
project(ApproximationTest)

set(CMAKE_CXX_STANDARD 11)

#standard libraries
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
#other libraries
set(CMAKE_EXE_LINKER_FLAGS "-static -lpthread")
set(SOURCE_FILES Main.cpp Stdafx.h Funcs.h Globals.h Branch.h PairConnectivity.cpp Branch.cpp Hypernet.h Hypernet.cpp
        Route.h Rote.cpp ApproximationAlgorithm.cpp)
add_executable(ApproximationTest ${SOURCE_FILES})