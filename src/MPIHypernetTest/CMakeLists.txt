cmake_minimum_required(VERSION 3.9)
project(MPIHypernetTest)
set(CMAKE_CXX_STANDARD 11)
#standard libraries
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
#other libraries
set(CMAKE_EXE_LINKER_FLAGS "-static -lpthread")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE_FILES Main.cpp Stdafx.h Funcs.h Globals.h DTO.h RandomHypernet.cpp
        SerializationExtensions.h HypernetBase/Stdafx.h HypernetBase/Branch.h
        HypernetBase/Branch.cpp HypernetBase/Hypernet.h HypernetBase/Hypernet.cpp HypernetBase/Route.H
        HypernetBase/Node.h HypernetBase/IIdentity.h HypernetBase/Reductions.cpp ApplicationSettings.h
        HypernetBase/Element.h HypernetBase/FactorizationHelpers.cpp HypernetBase/Node.cpp HypernetBase/Operators.h PairConnectivity.h)
add_executable(MPIHypernetTest ${SOURCE_FILES})
# Require MPI for this project:
find_package(MPI REQUIRED)
set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})
include_directories(${MPI_INCLUDE_PATH})
target_link_libraries(MPIHypernetTest ${MPI_LIBRARIES})
#Boost library
set(Boost_INCLUDE_DIR C:/Program\ Files/MinGW/include/)
set(Boost_LIBRARY_DIR C:/Program\ Files/MinGW/lib/)
find_package(Boost COMPONENTS serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(MPIHypernetTest ${Boost_LIBRARIES})