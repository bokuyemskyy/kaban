cmake_minimum_required(VERSION 3.20)


set(PROJECT_NAME "main")
set(EXECUTABLE_NAME "main")
set(LIBRARY_NAME "main_lib")

set(LIBS "imgui::imgui glfw OpenGL::GL rt pthread dl")
set(Stb_INCLUDE_DIR "${sourceDir}/vcpkg_installed/x64-linux/include/")
set(OpenGL_GL_PREFERENCE "GLVND")


project(${PROJECT_NAME})


#add_compile_options(
#    -Wall
#    -Werror
#    -Wextra
#    -Wundef
#    -Wunused
#    -Weffc++
#    -Wshadow
#    -Wformat
#    -Wcomment
#    -Wpedantic
#    -Wnarrowing
#    -Wconversion
#    -Wcast-align
#    -Wreturn-type
#    -Winvalid-pch
#    -Wunused-macros
#    -Wunused-result
#    -Wold-style-cast
#    -Wsign-conversion
#    -Wredundant-decls
#    -Wnull-dereference
#    -Wunused-parameter
#    -Wnon-virtual-dtor
#    -Wdouble-promotion
#    -Woverloaded-virtual
#    -Wimplicit-fallthrough
#    -Wmissing-declarations
#    -Wmissing-include-dirs
#    -Wmisleading-indentation
#    -Wmissing-format-attribute
#)


file(GLOB MAIN_SOURCE "project/main.cpp")
file(GLOB_RECURSE LIB_SOURCES "project/source/*.cpp")
file(GLOB_RECURSE TEST_SOURCES "project/tests/*.cpp")

separate_arguments(LIBS)

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(GTest CONFIG REQUIRED)
enable_testing()
include(GoogleTest)


file(GLOB_RECURSE HEADER_DIRS LIST_DIRECTORIES true ${CMAKE_SOURCE_DIR}/project/include/*)
set(INCLUDE_DIRS "")
foreach(path ${HEADER_DIRS})
    if(IS_DIRECTORY ${path})
        list(APPEND INCLUDE_DIRS ${path})
    endif()
endforeach()
include_directories(${INCLUDE_DIRS})


include_directories(SYSTEM ${VCPKG_INCLUDE_DIR})


if(LIB_SOURCES)
    add_library(${LIBRARY_NAME} ${LIB_SOURCES})
    if(LIBS)
        target_link_libraries(${LIBRARY_NAME} PRIVATE ${LIBS})
    endif()
endif()


if(EXECUTABLE_NAME)
    add_executable(${EXECUTABLE_NAME} ${MAIN_SOURCE})

    if(LIBS)
        target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${LIBS})
    endif()


    if(LIB_SOURCES)
        target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${LIBRARY_NAME})
    endif()

endif()

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})

    if(LIBS)
        target_link_libraries(${TEST_NAME} PRIVATE ${LIBS})
    endif()

    if(LIB_SOURCES)
        target_link_libraries(${TEST_NAME} PRIVATE ${LIBRARY_NAME})
    endif()

    target_link_libraries(${TEST_NAME} PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

    gtest_discover_tests(${TEST_NAME})
endforeach()














#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#    add_definitions(-DDEBUG)
#    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
#elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
#    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
#Pendif()
