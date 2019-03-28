cmake_minimum_required(VERSION 3.9)
project(HypernetTest)
set(CMAKE_CXX_STANDARD 11)
#standard libraries
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
#other libraries
set(CMAKE_EXE_LINKER_FLAGS "-static -lpthread")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE_FILES Main.cpp Funcs.h Globals.h FullEnumeration.cpp HypernetBase/Stdafx.h
        HypernetBase/Branch.h PairConnectivity.cpp HypernetBase/Branch.cpp HypernetBase/Hypernet.h
        HypernetBase/Hypernet.cpp HypernetBase/Route.h HypernetBase/Route.cpp HypernetBase/IIdentity.h
        HypernetBase/Reductions.cpp)
add_executable(HypernetTest ${SOURCE_FILES})