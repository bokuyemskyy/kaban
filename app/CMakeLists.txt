add_subdirectory(chess)

if(BUILD_UCI)
    add_subdirectory(interface/uci)
endif()
if(BUILD_GUI)
    add_subdirectory(interface/gui)
endif()