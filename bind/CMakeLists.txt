cmake_minimum_required(VERSION 3.10)
project(cdl)
set(CMAKE_CXX_STANDARD 17)
find_package(Python COMPONENTS Interpreter Development)

#Pybind11
#add_subdirectory(../headers)
add_subdirectory(pybind11)
pybind11_add_module(cdl
                    bind.cpp
                    ../headers/condorcet_domain.h
                    ../headers/utils.h
                    ../src/condorcet_domain.cpp
                    ../src/utils.cpp) # define the python module



