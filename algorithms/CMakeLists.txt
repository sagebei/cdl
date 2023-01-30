cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)
project(mcts)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/mcts.cpp
        ../core/src/condorcet_domain.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ../core/include ./include)


