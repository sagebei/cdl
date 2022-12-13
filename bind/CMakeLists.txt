cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)
project(cdl)

#Pybind11
add_subdirectory(pybind11)
find_package(Boost 1.74 COMPONENTS system filesystem serialization REQUIRED)

pybind11_add_module(cdl
                    bind.cpp
        ../core/src/condorcet_domain.cpp
        ../core/src/utils.cpp
        ../tools/src/tools.cpp) # define the python module

target_include_directories(cdl PUBLIC ../core/include ../tools/include)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(cdl PRIVATE ${Boost_LIBRARIES})


