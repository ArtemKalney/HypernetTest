cmake_minimum_required(VERSION 3.9)
project(OptimizationTest)
set(CMAKE_CXX_STANDARD 11)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE_FILES Main.cpp ../HypernetModel/Branch.cpp ../HypernetModel/Hypernet.cpp ../HypernetModel/Node.cpp
        ../HypernetModel/Helpers/DataHelper.cpp ../HypernetModel/Helpers/FactorizationHelper.cpp
        ../HypernetModel/Helpers/ReductionHelper.cpp ../HypernetTest/PairConnectivity.cpp)
add_executable(OptimizationTest ${SOURCE_FILES})
#standard libraries
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
#other libraries
set(CMAKE_EXE_LINKER_FLAGS "-static -lpthread")
#Boost library
set(Boost_INCLUDE_DIR C:/Program\ Files/MinGW/include/)
set(Boost_LIBRARY_DIR C:/Program\ Files/MinGW/lib/)
find_package(Boost COMPONENTS serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(OptimizationTest ${Boost_LIBRARIES})