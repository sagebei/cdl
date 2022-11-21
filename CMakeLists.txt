cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
find_package(Python COMPONENTS Interpreter Development)


project(cdl)
add_executable(cdl
        src/main.cpp
        src/condorcet_domain.cpp
        src/utils.cpp)
target_include_directories(cdl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


