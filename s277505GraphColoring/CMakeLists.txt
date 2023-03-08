cmake_minimum_required(VERSION 3.21)
project(s277505GraphColoring)

set(CMAKE_CXX_STANDARD 20)

add_executable(s277505GraphColoring main.cpp Graph/Graph.h Graph/Graph.cpp Parser/DimacsP.cpp Parser/DimacsP.h Utility/ProdConsV.h Utility/Splitter.h ColoringMethods/Luby.h ColoringMethods/Luby.cpp ColoringMethods/Greedy.h ColoringMethods/Greedy.cpp ColoringMethods/Solver.h Benchmark/Benchmark.h Benchmark/Benchmark.cpp ColoringMethods/Jones.cpp ColoringMethods/Jones.h ColoringMethods/LDF.cpp ColoringMethods/LDF.h ColoringMethods/SDL.cpp ColoringMethods/SDL.h)
include_directories(ColoringMethods)
include_directories(Graph)
include_directories(Parser)
include_directories(Utility)
include_directories(Benchmark)
