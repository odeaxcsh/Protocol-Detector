cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Protocol-detector VERSION 1.0)

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

file(GLOB SOURCES "sources/*.cpp")
file(GLOB TESTS "tests/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES} protocol_detector.cpp)
target_include_directories( ${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include )


add_executable(Project_test ${TESTS} ${SOURCES})
target_include_directories( Project_test PRIVATE ${PROJECT_SOURCE_DIR}/include )

#unit test
target_link_libraries(Project_test gtest gtest_main pthread)
add_test(
    NAME Project_test
    COMMAND Project_test
)
