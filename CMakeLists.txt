cmake_minimum_required(VERSION 3.23)
project(cd)

#add_subdirectory(pybind11)
#pybind11_add_module(condorcet bind.cpp) # define the python module

set(CMAKE_CXX_STANDARD 17)

add_executable(cd main.cpp condorcet_domain.cpp condorcet_domain.h)