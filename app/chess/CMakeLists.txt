file(GLOB_RECURSE SOURCES "*.cpp")

add_library(kaban_lib STATIC ${SOURCES})

file(GLOB_RECURSE HEADER_DIRS LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(path ${HEADER_DIRS})
    if(IS_DIRECTORY ${path})
        list(APPEND INCLUDE_DIRS ${path})
    endif()
endforeach()
set(INCLUDE_DIRS ${INCLUDE_DIRS})

target_include_directories(kaban_lib PUBLIC ${INCLUDE_DIRS})

if (MSVC)
    target_compile_options(kaban_lib PUBLIC
        /W4
        /WX
        /permissive-
    )
else()
    target_compile_options(kaban_lib PUBLIC
        -Wall
        -Wextra
        -Werror
        -Wundef
        -Wunused
        -Weffc++
        -Wshadow
        -Wformat
        -Wcomment
        -Wpedantic
        -Wnarrowing
        -Wcast-align
        -Wreturn-type
        -Winvalid-pch
        -Wunused-macros
        -Wunused-result
        -Wold-style-cast
        -Wsign-conversion
        -Wredundant-decls
        -Wnull-dereference
        -Wunused-parameter
        -Wnon-virtual-dtor
        -Wdouble-promotion
        -Woverloaded-virtual
        -Wimplicit-fallthrough
        -Wmissing-declarations
        -Wmissing-include-dirs
        -Wmisleading-indentation
        -Wmissing-format-attribute
        # -Wconversion
    )
endif()