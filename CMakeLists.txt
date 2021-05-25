cmake_minimum_required(VERSION 3.10)

# set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Protocol-detector VERSION 1.0)

file(GLOB SOURCES "sources/*.cpp")
add_library(main_lib SHARED ${SOURCES})

# set executable path to build/bin
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

# set project main file
set(PROJECT_MAIN_FILE "protocol_detector.cpp")

# add jsoncpp external project
if(EXISTS "${PROJECT_SOURCE_DIR}/extern/jsoncpp/")
    add_subdirectory("${PROJECT_SOURCE_DIR}/extern/jsoncpp/" EXCLUDE_FROM_ALL)

    # include JSONCPP
    get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
    include_directories(${JSON_INC_PATH})
    
else()
		message(FATAL_ERROR "Local JSONCPP submodule not found. To download with Git, run git submodule update --init")
endif()

# add libtins external project
if(EXISTS "${PROJECT_SOURCE_DIR}/extern/libtins/")
    add_subdirectory("${PROJECT_SOURCE_DIR}/extern/libtins" EXCLUDE_FROM_ALL)

    # include LIBTINS file
    include_directories (
        SYSTEM
        ${libtins_SOURCE_DIR}
        ${libtins_SOURCE_DIR}/include
    )
else()
		message(FATAL_ERROR "Local LIBTINS submodule not found. To download with Git, run git submodule update --init")
endif()

message(STATUS "external projects are complete")

include_directories("${PROJECT_SOURCE_DIR}/include/")

add_executable(${PROJECT_NAME} ${PROJECT_MAIN_FILE})
target_link_libraries(${PROJECT_NAME} main_lib jsoncpp_lib tins)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    enable_testing()
    include(GoogleTest)
    add_subdirectory("${PROJECT_SOURCE_DIR}/tests/")
endif()
