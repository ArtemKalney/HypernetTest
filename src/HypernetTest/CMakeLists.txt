cmake_minimum_required(VERSION 3.9)
project(HypernetTest)
set(CMAKE_CXX_STANDARD 11)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE_FILES Main.cpp Funcs.h Globals.h Stdafx.h
        ../HypernetModel/Branch.h PairConnectivity.cpp ../HypernetModel/Branch.cpp ../HypernetModel/Hypernet.h
        ../HypernetModel/Hypernet.cpp ../HypernetModel/Route.h
        ../HypernetModel/Helpers/ReductionHelper.cpp Tests.cpp ../HypernetModel/Node.h
        ../HypernetModel/Node.cpp
        ComputeMENC.h ComputeAPC.h ../HypernetModel/Helpers/FactorizationHelper.cpp FullEnumeration.cpp ../HypernetModel/IIdentity.h ApplicationSettings.h ../HypernetModel/Element.h ../HypernetModel/Operators.h
        ../HypernetModel/Helpers/RandomHypernetHelper.cpp)
add_executable(HypernetTest ${SOURCE_FILES})
#standard libraries
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
#other libraries
set(CMAKE_EXE_LINKER_FLAGS "-static -lpthread")
#googletest
add_subdirectory(libs/googletest)
include_directories(libs/googletest/include)
target_link_libraries(HypernetTest gtest gtest_main)
#Boost library
set(Boost_INCLUDE_DIR C:/Program\ Files/MinGW/include/)
set(Boost_LIBRARY_DIR C:/Program\ Files/MinGW/lib/)
find_package(Boost COMPONENTS serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(HypernetTest ${Boost_LIBRARIES})