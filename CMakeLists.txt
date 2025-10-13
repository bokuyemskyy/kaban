cmake_minimum_required(VERSION 3.30)

# initialization
project("kaban")

# setting up compiler
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)
set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")

# options
option(BUILD_UCI "Build UCI" ON)
option(BUILD_GUI "Build GUI" ON)
option(BUILD_SCRIPTS "Build scripts" OFF)
option(BUILD_TESTS "Build tests" OFF)

add_subdirectory(app)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(BUILD_SCRIPTS)
    add_subdirectory(scripts)
endif()