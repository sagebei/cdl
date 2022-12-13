cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)
project(tools)

find_package(Boost)

add_executable(tools
        src/main.cpp
        ../src/condorcet_domain.cpp
        ../src/utils.cpp)

include_directories(${Boost_INCLUDE_DIR})
target_include_directories(tools PUBLIC ../include ./include)



