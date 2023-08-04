# cmake for ezpsf tests

set(EZPSF_TEST_LINK_LIBS GTest::gtest_main fishstore)


FUNCTION(ADD_EZPSF_TEST TEST_NAME)
    add_executable(${TEST_NAME} test_adapter.h basic_ezpsf_fixture.h ${TEST_NAME}.cpp)
    target_link_libraries(${TEST_NAME} ${EZPSF_TEST_LINK_LIBS})
    target_compile_options(${TEST_NAME} PRIVATE -UNDEBUG)
    gtest_discover_tests(${TEST_NAME})
ENDFUNCTION()

ADD_EZPSF_TEST(basic_exprs_test)

FUNCTION(ADD_EZPSF_FISHSTORE_TEST TEST_NAME HEADERS)
    add_executable(${TEST_NAME}_EzPsf ../${TEST_NAME}.cc)
    target_link_libraries(${TEST_NAME}_EzPsf ${FISHSTORE_TEST_LINK_LIBS} simdjson)
    target_compile_definitions(${TEST_NAME}_EzPsf PUBLIC USE_EZPSF)
    if (MSVC)
        target_compile_options(${TEST_NAME}_EzPsf PRIVATE /UNDEBUG)
    else()
        target_compile_options(${TEST_NAME}_EzPsf PRIVATE -UNDEBUG)
    endif()
    gtest_discover_tests(${TEST_NAME}_EzPsf)
ENDFUNCTION()

ADD_EZPSF_FISHSTORE_TEST(in_memory_test "")