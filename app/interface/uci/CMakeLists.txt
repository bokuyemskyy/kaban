add_executable(kaban_uci
    main.cpp
)

target_link_libraries(kaban_uci PRIVATE kaban_lib)
target_include_directories(kaban_uci PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
