cmake_minimum_required(VERSION 3.9)
project(OptimizationTest)
set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE_FILES Main.cpp ../HypernetModel/Branch.cpp ../HypernetModel/Hypernet.cpp ../HypernetModel/Node.cpp
        ../HypernetModel/Helpers/DataHelper.cpp ../HypernetModel/Helpers/FactorizationHelper.cpp
        ../HypernetModel/Helpers/ReductionHelper.cpp ../HypernetTest/PairConnectivity.cpp
        ../HypernetModel/Generators/RandomHypernetGenerator.cpp ../HypernetModel/Element.h ../HypernetModel/IIdentity.h
        ../HypernetModel/Node.h ../HypernetModel/Operators.h ../HypernetModel/Route.h ApplicationSettings.h
        Funcs.h Globals.h Stdafx.h "Model .h" Model.cpp SimulatedAnnealingAlgorithm.h SimulatedAnnealingAlgorithm.cpp
        FullEnumerationAlgorithm.h FullEnumerationAlgorithm.cpp ../HypernetModel/Helpers/InputParser.h
        ../HypernetModel/Helpers/Settings.h)
add_executable(OptimizationTest ${SOURCE_FILES})

#standard libraries
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

#other libraries
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "-static -lpthread")
endif()

#Boost library
find_package(Boost COMPONENTS serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(OptimizationTest ${Boost_LIBRARIES})