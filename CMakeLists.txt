cmake_minimum_required(VERSION 3.20)

# initialization
project("kaban")

# scripts
add_subdirectory(scripts/compare_perft)

# release optimization
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
foreach(var
        CMAKE_C_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_EXE_LINKER_FLAGS_RELEASE
        CMAKE_SHARED_LINKER_FLAGS_RELEASE)
    set(${var} "${${var}} -O3 -march=native -flto -DNDEBUG"
        CACHE STRING "Tuned for performance" FORCE)
endforeach()

# compile options for code clarity 
add_compile_options(
   #-Wall
    #-Werror
    #-Wextra
    #-Wundef
    #-Wunused
    #-Weffc++
    #-Wshadow
    #-Wformat
    #-Wcomment
    #-Wpedantic
    #-Wnarrowing
    #-Wcast-align
    #-Wreturn-type
    #-Winvalid-pch
    #-Wunused-macros
    #-Wunused-result
    #-Wold-style-cast
    #-Wsign-conversion
    #-Wredundant-decls
    #-Wnull-dereference
    #-Wunused-parameter
    #-Wnon-virtual-dtor
    #-Wdouble-promotion
    #-Woverloaded-virtual
    #-Wimplicit-fallthrough
    #-Wmissing-declarations
    #-Wmissing-include-dirs
    #-Wmisleading-indentation
    #-Wmissing-format-attribute
    #    -Wconversion
)

# searching for sources
file(GLOB MAIN_SOURCE "app/main.cpp")
file(GLOB_RECURSE LIB_SOURCES "app/*/*.cpp")
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

# configuring packages
set(Stb_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-linux/include/")
set(OpenGL_GL_PREFERENCE "GLVND")

# including packages
set(PACKAGES "imgui::imgui glfw OpenGL::GL rt pthread dl")
separate_arguments(PACKAGES)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(GTest CONFIG REQUIRED)

# enabling testing
enable_testing()
include(GoogleTest)

# configuring headers
file(GLOB_RECURSE HEADER_DIRS LIST_DIRECTORIES true ${CMAKE_SOURCE_DIR}/app/*)
set(INCLUDE_DIRS "")
foreach(path ${HEADER_DIRS})
    if(IS_DIRECTORY ${path})
        list(APPEND INCLUDE_DIRS ${path})
    endif()
endforeach()
include_directories(${INCLUDE_DIRS})

# building library
if(LIB_SOURCES)
    add_library("kaban_lib" ${LIB_SOURCES})
    if(PACKAGES)
        target_link_libraries("kaban_lib" PRIVATE ${PACKAGES})
    endif()
endif()

# building executable
add_executable("kaban" ${MAIN_SOURCE})
if(PACKAGES)
    target_link_libraries("kaban" PRIVATE ${PACKAGES})
endif()

if(LIB_SOURCES)
    target_link_libraries("kaban" PRIVATE "kaban_lib")
endif()

# building tests
if(TEST_SOURCES)
    add_executable("kaban_tests" ${TEST_SOURCES})

    if(LIBS)
        target_link_libraries("kaban_tests" PRIVATE ${PACKAGES})
    endif()

    if(LIB_SOURCES)
        target_link_libraries("kaban_tests" PRIVATE "kaban_lib")
    endif()

    target_link_libraries("kaban_tests" PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

    gtest_discover_tests("kaban_tests")
endif()














#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#    add_definitions(-DDEBUG)
#    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
#elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
#    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
#Pendif()
