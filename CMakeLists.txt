cmake_minimum_required(VERSION 3.23)
project(cd)
set(CMAKE_CXX_STANDARD 17)

# Pybind11
add_subdirectory(pybind11)
pybind11_add_module(cd condorcet_domain.h condorcet_domain.cpp bind.cpp) # define the python module

#add_executable(cd main.cpp condorcet_domain.cpp condorcet_domain.h)