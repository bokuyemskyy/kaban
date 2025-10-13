file(GLOB MAIN_SOURCE "main.cpp")
file(GLOB_RECURSE LIB_SOURCES "*/*.cpp")

file(GLOB_RECURSE HEADER_DIRS LIST_DIRECTORIES true ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(path ${HEADER_DIRS})
    if(IS_DIRECTORY ${path})
        list(APPEND INCLUDE_DIRS ${path})
    endif()
endforeach()
set(INCLUDE_DIRS ${INCLUDE_DIRS} PARENT_SCOPE)

message(STATUS "Include directories: ${INCLUDE_DIRS}")

if(LIB_SOURCES)
    add_library(kaban_lib ${LIB_SOURCES})
    if(PACKAGES)
        target_link_libraries(kaban_lib PUBLIC ${PACKAGES})
        target_include_directories(kaban_lib PUBLIC ${INCLUDE_DIRS})

        # compile options for code clarity
        target_compile_options(kaban_lib PUBLIC
            -Wall
            -Werror
            -Wextra
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
endif()

add_executable("kaban" ${MAIN_SOURCE})
target_include_directories(kaban PRIVATE ${INCLUDE_DIRS})
if(PACKAGES)
    target_link_libraries("kaban" PRIVATE ${PACKAGES})
endif()
    
if(LIB_SOURCES)
    target_link_libraries("kaban" PRIVATE "kaban_lib")
endif()