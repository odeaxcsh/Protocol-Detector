cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Protocol-detector VERSION 1.0)

set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

file(GLOB SOURCES "sources/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES} protocol_detector.cpp)
target_include_directories( ${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include )

set(Gtest_essential gtest gtest_main pthread)

find_library(GTEST gtest)
if(NOT GTEST)
    message(WARNING "Google test is not installed, unit tests could not be generated!")
else()
    file(GLOB TESTS "tests/*.cpp")
    add_executable(Project_test ${SOURCES} ${TESTS})
    target_include_directories( Project_test PRIVATE ${PROJECT_SOURCE_DIR}/include )
    target_link_libraries(Project_test gtest ${Gtest_essential})
    add_test(
        NAME Project_test
        COMMAND Project_test
    )
endif()
