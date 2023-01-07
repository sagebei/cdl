cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)


project(core)
add_executable(core
        ./src/main.cpp
        ./src/condorcet_domain.cpp
        ./src/wrapper.cpp
        ./src/utils.cpp)
target_include_directories(core PUBLIC ./include)


