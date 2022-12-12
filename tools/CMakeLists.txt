cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)


project(tool)
add_executable(tool
        main.cpp
        ../src/condorcet_domain.cpp
        ../src/utils.cpp)
target_include_directories(tool PUBLIC ../include)


