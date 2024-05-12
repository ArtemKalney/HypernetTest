cmake_minimum_required(VERSION 3.9)
project(HypernetTest)
set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE_FILES Main.cpp PairConnectivity.cpp ../HypernetModel/Branch.cpp ../HypernetModel/Hypernet.cpp
        ../HypernetModel/Helpers/ReductionHelper.cpp Tests.cpp ../HypernetModel/Node.cpp
        ../HypernetModel/Helpers/FactorizationHelper.cpp FullEnumeration.cpp
        ../HypernetModel/Generators/RandomHypernetGenerator.cpp ../HypernetModel/Helpers/DataHelper.cpp
        ApplicationSettings.h ComputeAPC.h ComputeMENC.h Funcs.h Globals.h Stdafx.h ../HypernetModel/Branch.h
        ../HypernetModel/Element.h ../HypernetModel/IIdentity.h ../HypernetModel/Node.h ../HypernetModel/Operators.h
        ../HypernetModel/Route.h ../HypernetModel/Helpers/InputParser.h ../HypernetModel/Helpers/Settings.h
        ApproximationAlgorithm.cpp)
add_executable(HypernetTest ${SOURCE_FILES})

#standard libraries
if (WIN32)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

#other libraries
if (WIN32)
set(CMAKE_EXE_LINKER_FLAGS "-static -lpthread")
endif()

#googletest
add_subdirectory(libs/googletest)
include_directories(libs/googletest/include)
target_link_libraries(HypernetTest gtest gtest_main)

#Boost library
find_package(Boost COMPONENTS serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(HypernetTest ${Boost_LIBRARIES})