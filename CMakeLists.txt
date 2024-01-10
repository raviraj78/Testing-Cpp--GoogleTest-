cmake_minimum_required(VERSION 3.10)

project(GTESTProject)

set(SOURCES src/main.cpp
src/provider.cpp
src/user.cpp
src/client.cpp
src/server.cpp)

# file(GLOB_RECURSE SOURCES src/*.cpp)

include_directories(include)
include(GoogleTest)

set(TEST_SOURCES tests/test_provider.cpp)
# set(TEST_SOURCES tests/test_user.cpp)
# file(GLOB_RECURSE TEST_SOURCES GTEST/tests/*.cpp)

add_executable(your_project ${SOURCES})
# add_executable(your_tests ${TEST_SOURCES})
add_executable(your_tests
    tests/test_provider.cpp
    src/provider.cpp
    )
# add_executable(your_tests
#     tests/test_user.cpp
#     src/user.cpp
#     )

file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(your_tests gtest gtest_main gmock gmock_main)

enable_testing()

gtest_discover_tests(your_tests)
add_test(NAME your_tests COMMAND your_tests)