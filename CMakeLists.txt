cmake_minimum_required(VERSION 3.20)

# initialization
project("kaban")

# options
option(BUILD_SCRIPTS "Build scripts" OFF)
option(BUILD_TESTS "Build tests" OFF)

# setting up compiler
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)
set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0 -flto=auto")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto=auto -DNDEBUG")

# configuring packages
set(PACKAGES "imgui::imgui glfw OpenGL::GL rt pthread dl")
separate_arguments(PACKAGES)

set(Stb_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-linux/include/")
set(OpenGL_GL_PREFERENCE "GLVND")

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(GTest CONFIG REQUIRED)

# building main library and executable
add_subdirectory(app)
        
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

if(BUILD_SCRIPTS)
    add_subdirectory(scripts)
endif()