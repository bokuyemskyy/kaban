add_executable(uci
    main.cpp
)

target_link_libraries(uci PRIVATE kaban_lib)
target_include_directories(uci PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
