cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)


project(cdl)
add_executable(cdl
        src/main.cpp
        src/condorcet_domain.cpp
        src/utils.cpp tools/tool.cpp tools/tools.h)
target_include_directories(cdl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


