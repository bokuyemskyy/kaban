cmake_minimum_required(VERSION 3.20)
project(compare_perft LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost 1.83 REQUIRED COMPONENTS system filesystem)

add_executable(compare_perft main.cpp)
target_include_directories(compare_perft PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(compare_perft PRIVATE ${Boost_LIBRARIES} boost_process)