cmake_minimum_required(VERSION 3.19)
project(untitled)

set(CMAKE_CXX_STANDARD 17)
set(BOOST_ROOT "C:/C++Lib/vcpkg/installed/x64-windows")

find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(untitled main.cpp)

target_link_libraries(untitled ${Boost_LIBRARIES})
