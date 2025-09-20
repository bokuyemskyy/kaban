cmake_minimum_required(VERSION 3.20)

project(compare_perft LANGUAGES CXX)

add_executable(compare_perft main.cpp)

find_package(boost_process CONFIG REQUIRED)
target_link_libraries(compare_perft PRIVATE Boost::process)