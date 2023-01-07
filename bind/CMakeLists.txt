cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)
project(cdl)

#Pybind11
add_subdirectory(pybind11)

pybind11_add_module(cdl
                    bind.cpp
        ../core/src/condorcet_domain.cpp
        ../core/src/wrapper.cpp
        ../core/src/utils.cpp) # define the python module

target_include_directories(cdl PUBLIC ../core/include)


