# cmake for ezpsf tests

set(EZPSF_TEST_LINK_LIBS GTest::gtest_main LLVM-14 antlr4-runtime)


FUNCTION(ADD_EZPSF_TEST TEST_NAME)
    add_executable(${TEST_NAME} basic_ezpsf_fixture.h ${TEST_NAME}.cpp)
    target_link_libraries(${TEST_NAME} ${EZPSF_TEST_LINK_LIBS})
    target_compile_options(${TEST_NAME} PRIVATE -UNDEBUG)
    gtest_discover_tests(${TEST_NAME})
ENDFUNCTION()

ADD_EZPSF_TEST(basic_exprs_test)