cmake_policy(SET CMP0167 NEW)

find_package(Boost 1.83 REQUIRED COMPONENTS system filesystem)

add_executable(compare_perft main.cpp)
target_include_directories(compare_perft PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(compare_perft PRIVATE ${Boost_LIBRARIES} boost_process)