cmake_minimum_required(VERSION 3.9)
set(CMAKE_CXX_STANDARD 17)
project(tools)

find_package(Boost 1.74 COMPONENTS system filesystem serialization REQUIRED)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/tools.cpp
        ../core/src/condorcet_domain.cpp
        ../core/src/utils.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ../core/include ./include)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})


