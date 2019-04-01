cmake_minimum_required(VERSION 3.9)
project(HypernetTest)
set(CMAKE_CXX_STANDARD 11)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE_FILES Main.cpp Funcs.h Globals.h FullEnumeration.cpp HypernetBase/Stdafx.h
        HypernetBase/Branch.h PairConnectivity.cpp HypernetBase/Branch.cpp HypernetBase/Hypernet.h
        HypernetBase/Hypernet.cpp HypernetBase/Route.h HypernetBase/IIdentity.h
        HypernetBase/Reductions.cpp Tests.cpp HypernetBase/Node.h)
add_executable(HypernetTest ${SOURCE_FILES})
#standard libraries
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
#other libraries
set(CMAKE_EXE_LINKER_FLAGS "-static -lpthread")
#googletest
add_subdirectory(libs/googletest)
include_directories(libs/googletest/include)
target_link_libraries(HypernetTest gtest gtest_main)