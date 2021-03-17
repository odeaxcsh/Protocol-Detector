cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Protocol-detector VERSION 1.0)

file(GLOB SOURCES "sources/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES} protocol_detector.cpp)

target_include_directories(
    ${PROJECT_NAME}
        PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
