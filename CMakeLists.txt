cmake_minimum_required(VERSION 3.15...3.29)
set(CMAKE_CXX_STANDARD 17)
project(cdl)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(cdl
                    bind/bind.cpp
        core/src/condorcet_domain.cpp
        core/src/forbidden_permutation.cpp
        core/src/wrapper.cpp
        core/src/utils.cpp) # define the python module

target_include_directories(cdl PUBLIC core/include)
install(TARGETS cdl DESTINATION .)

# export MACOSX_DEPLOYMENT_TARGET=11.0
# python3 -m build --sdist 
# python3 -m cibuildwheel --output-dir dist 
# python3 -m cibuildwheel --platform linux --output-dir dist 
# python3 -m cibuildwheel --platform macos --output-dir dist 
# twine upload dist/*