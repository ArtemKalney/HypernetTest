cmake_minimum_required(VERSION 3.9)
project(HypernetTest)
set(CMAKE_CXX_STANDARD 11)

set(Boost_DIR C:/Users/Compucter/libs/MinGW)
set(Boost_INCLUDE_DIR ${Boost_DIR}/include/)
set(Boost_LIBRARY_DIR ${Boost_DIR}/lib/)

add_subdirectory(HypernetTest)
add_subdirectory(MPIHypernetTest)
add_subdirectory(OptimizationTest)