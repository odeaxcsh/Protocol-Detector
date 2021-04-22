cmake_minimum_required(VERSION 3.10)

# set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Protocol-detector VERSION 1.0)

# Configure google-test as a downloadable library.
include(External_GTest.cmake)

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

set(PROJECT_MAIN_FILE "protocol_detector.cpp")
set(TEST_MAIN_FILE "tests/run_tests.cpp")

set(PROJECT_TEST "${PROJECT_NAME}-Test")

#find JsonCPP
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

file(GLOB SOURCES "sources/*.cpp")
file(GLOB TESTS "tests/*.cpp")

add_executable(${PROJECT_TEST} ${TEST_MAIN_FILE} ${TESTS} ${SOURCES})
add_executable(${PROJECT_NAME} ${PROJECT_MAIN_FILE} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE  "${PROJECT_SOURCE_DIR}/include" "${JSONCPP_INCLUDE_DIRS}")
target_include_directories(${PROJECT_TEST} PRIVATE  "${PROJECT_SOURCE_DIR}/include" "${JSONCPP_INCLUDE_DIRS}")

target_link_libraries(${PROJECT_NAME} ${JSONCPP_LIBRARIES})
target_link_libraries(${PROJECT_TEST} ${GTEST_LIBRARY} ${GMOCK_LIBRARY} ${JSONCPP_LIBRARIES})

add_test(NAME ${PROJECT_TEST} COMMAND ${PROJECT_TEST})
