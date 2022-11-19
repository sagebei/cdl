cmake_minimum_required(VERSION 3.23)
project(cdl)
set(CMAKE_CXX_STANDARD 17)
find_package(Python COMPONENTS Interpreter Development)

add_executable(cdl
        src/main.cpp
        src/condorcet_domain.cpp
        headers/condorcet_domain.h
        headers/utils.h
        src/utils.cpp)



