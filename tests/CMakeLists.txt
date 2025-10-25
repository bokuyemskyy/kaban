find_package(GTest CONFIG REQUIRED)

enable_testing()
include(GoogleTest)

file(GLOB_RECURSE TEST_SOURCES "*.cpp")

add_executable(kaban_tests ${TEST_SOURCES})

if(PACKAGES)
    target_link_libraries(kaban_tests PRIVATE ${PACKAGES})
endif()

target_link_libraries(kaban_tests PRIVATE kaban_lib GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

gtest_discover_tests(kaban_tests)
