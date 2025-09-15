cmake_minimum_required(VERSION 3.20)

project(perft_compare LANGUAGES CXX)

add_executable(perft_compare main.cpp)

find_package(boost_process CONFIG REQUIRED)
target_link_libraries(perft_compare PRIVATE Boost::process)