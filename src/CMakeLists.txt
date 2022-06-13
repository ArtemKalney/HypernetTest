cmake_minimum_required(VERSION 3.9)
project(HypernetTest)
set(CMAKE_CXX_STANDARD 11)

set(Boost_DIR C:/Users/Compucter/libs/MinGW/)

add_subdirectory(HypernetTest)
add_subdirectory(MPIHypernetTest)
#add_subdirectory(ApproximationTest)
add_subdirectory(OptimizationTest)