file(GLOB_RECURSE SOURCES "*.cpp")

file(GLOB_RECURSE HEADER_DIRS LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(path ${HEADER_DIRS})
    if(IS_DIRECTORY ${path})
        list(APPEND INCLUDE_DIRS ${path})
    endif()
endforeach()
set(INCLUDE_DIRS ${INCLUDE_DIRS})


separate_arguments(PACKAGES)

set(OpenGL_GL_PREFERENCE "GLVND")
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(GTest CONFIG REQUIRED)


add_executable("kaban_gui" ${SOURCES})
message(${CMAKE_CURRENT_SOURCE_DIR})
message(${INCLUDE_DIRS})
target_include_directories(kaban_gui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${INCLUDE_DIRS})

if(WIN32)
    target_link_libraries("kaban_gui" PRIVATE kaban_lib imgui::imgui glfw opengl32)
    elseif(UNIX)
        target_link_libraries("kaban_gui" PRIVATE kaban_lib imgui::imgui glfw OpenGL::GL rt pthread dl)
endif()
