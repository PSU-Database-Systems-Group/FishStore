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
    add_executable(${TEST_NAME}_ezpsf ../${TEST_NAME}.cc)
    target_link_libraries(${TEST_NAME}_ezpsf ${FISHSTORE_TEST_LINK_LIBS} simdjson)
    target_compile_definitions(${TEST_NAME}_ezpsf PUBLIC USE_EZPSF)
    if (MSVC)
        target_compile_options(${TEST_NAME}_ezpsf PRIVATE /UNDEBUG)
    else ()
        target_compile_options(${TEST_NAME}_ezpsf PRIVATE -UNDEBUG)
    endif ()
    gtest_discover_tests(${TEST_NAME}_ezpsf)
ENDFUNCTION()

ADD_EZPSF_FISHSTORE_TEST(in_memory_test "")
ADD_EZPSF_FISHSTORE_TEST(ingest_queue_test "ingest_test.h")
if (MSVC)
    ADD_EZPSF_FISHSTORE_TEST(ingest_threadpool_test "ingest_test.h")
endif ()
ADD_EZPSF_FISHSTORE_TEST(register_test "")
ADD_EZPSF_FISHSTORE_TEST(checkpoint_queue_test "checkpoint_test.h")
if (MSVC)
    ADD_EZPSF_FISHSTORE_TEST(checkpoint_threadpool_test "checkpoint_test.h")
endif ()