# include googletest submodule
add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest")

# following command causes cleaner cache
mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

# for IDE support
set_target_properties(gtest PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)
set_target_properties(gmock PROPERTIES FOLDER extern)
set_target_properties(gmock_main PROPERTIES FOLDER extern)

# find utils


# a simpler function to add tests
macro(quick_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN} "${PROJECT_SOURCE_DIR}/tests/unittesting_utils.cpp")
    target_include_directories(${TESTNAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")
    target_link_libraries(${TESTNAME} main_lib gtest gmock gtest_main jsoncpp_lib)
    gtest_discover_tests(
        ${TESTNAME}
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
    )
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

# include Gtest
include_directories (
    SYSTEM
    ${gtest_SOURCE_DIR}
    ${gtest_SOURCE_DIR}/include
)

# find all cpp files
file(GLOB_RECURSE test_cases *unittest.cpp)

# add an excutable and test for each file
foreach(case_file ${test_cases})
    get_filename_component(case_name ${case_file} NAME_WE)
    set(case_name test_${case_name})
    quick_add_test(${case_name} ${case_file})
endforeach()