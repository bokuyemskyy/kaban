enable_testing()
include(GoogleTest)
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")

add_executable(kaban_tests ${TEST_SOURCES})

if(PACKAGES)
    target_link_libraries(kaban_tests PRIVATE ${PACKAGES})
endif()

if(LIB_SOURCES)
    target_link_libraries(kaban_tests PRIVATE kaban_lib)
endif()

target_link_libraries(kaban_tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

gtest_discover_tests(kaban_tests)