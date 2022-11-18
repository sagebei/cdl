cmake_minimum_required(VERSION 3.23)
project(cdl)
set(CMAKE_CXX_STANDARD 17)
find_package(Python COMPONENTS Interpreter Development)

# Pybind11
add_subdirectory(pybind11)
pybind11_add_module(cdl condorcet_domain.h condorcet_domain.cpp utils.h utils.cpp bind.cpp) # define the python module

#add_executable(cdl main.cpp condorcet_domain.cpp condorcet_domain.h utils.h utils.cpp)



