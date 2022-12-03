cmake_minimum_required(VERSION 3.9)
project(cdl)
set(CMAKE_CXX_STANDARD 17)

#Pybind11
add_subdirectory(pybind11)
pybind11_add_module(cdl
                    bind.cpp
                    ../src/condorcet_domain.cpp
                    ../src/utils.cpp) # define the python module
target_include_directories(cdl PUBLIC ../include)



