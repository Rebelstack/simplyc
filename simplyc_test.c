/**
 * @file simplyc_test.c
 *
 * @brief Entry point for the program used to test the SimplyC unit
 * testing framework.
 *
 * This program is free software. You can redistribute it and/or modify it
 * under the terms of the GNU General Public License, version 3 (GPLv3).
 */
#include <stdbool.h>
#include <stdint.h>
#include "unit_test.h"
#include "simplyc_test.h"

// static function declarations
static void test_suite_usage(void);
static void test_case_usage(void);
static void test_unit_test(void);
static void test_boolean_asserts(void);
static void test_int8_asserts(void);
static void test_uint8_asserts(void);
static void test_int16_asserts(void);
static void test_uint16_asserts(void);
static void test_int32_asserts(void);
static void test_uint32_asserts(void);
static void test_int64_asserts(void);
static void test_uint64_asserts(void);
static void test_float32_asserts(void);
static void test_float64_asserts(void);

/**
 * Entry point of SimplyC unit testing tests.
 */
void simplyc_test (void)
{
    /**
     * @attention it is assumed that this file is built within a project
     * that defines UNIT_TEST_INT64 and UNIT_TEST_FLOATING_POINT, SimplyC
     * allows support for these to be conditionally compiled, to test them,
     * must include them
     */
     
    // test the SimplyC unit test framework, turn logging on
    unit_test_log_on("simplyc_test_output.txt");

    // test that incorrect usage of the unit test framework is detected
    // and reported
    test_suite_usage();
    test_case_usage();

    // call all the unit test assertion functions and verify they work
    // as expected
    test_unit_test();

    // clean up and turn logging off
    unit_test_log_off();
}

/**
 * Test the SimplyC test suite framework.
 */
static void test_suite_usage (void)
{
    // start and end a test suite correctly
    test_suite_start("test_suite_a");
    test_suite_end();

    // only 1 test suite is allowed at a time, make sure this is detected
    test_suite_start("test_suite_b");
    test_suite_start("test_suite_c");
    test_suite_end();

    // try and end a test suite when one is not active
    test_suite_start("test_suite_d");
    test_suite_end();
    test_suite_end();
}

/**
 * Test the SimplyC test case framework.
 */
static void test_case_usage (void)
{
    // start and end a test suite correctly
    test_case_start("test_case_a");
    test_case_end();

    // only 1 test case is allowed at a time, make sure this is detected
    test_case_start("test_case_b");
    test_case_start("test_case_c");
    test_case_end();

    // try and end a test case when one is not active
    test_case_start("test_case_d");
    test_case_end();
    test_case_end();
}

/**
 * Test all the unit test assertion functions.
 */
 static void test_unit_test (void)
 {
    test_suite_start("Unit test assertion verification");
    test_boolean_asserts();
    test_int8_asserts();
    test_uint8_asserts();
    test_int16_asserts();
    test_uint16_asserts();
    test_int32_asserts();
    test_uint32_asserts();
    test_int64_asserts();
    test_uint64_asserts();
    test_float32_asserts();
    test_float64_asserts();
    test_suite_end();
 }

/**
 * Test the SimplyC boolean assertions.
 */
static void test_boolean_asserts (void)
{
    // test the boolean type, pass and fail
    test_case_start("Test boolean asserts, these should pass");

    bool actual   = true;
    bool expected = actual;
    ASSERT_BOOL_EQ(expected, actual);
    expected = !actual;
    ASSERT_BOOL_NOT_EQ(expected, actual);

    test_case_end();

    test_case_start("Test boolean asserts, these should fail");

    actual   = false;
    expected = !actual;
    ASSERT_BOOL_EQ(expected, actual);
    expected = actual;
    ASSERT_BOOL_NOT_EQ(expected, actual);

    test_case_end();
}

/**
 * Test the SimplyC int8 assertions.
 */
static void test_int8_asserts (void)
{
    // test the int8 type, pass and fail
    test_case_start("Test int8 asserts, these should pass");

    int8_t actual   = INT8_MAX;
    int8_t expected = actual;
    ASSERT_INT8_EQ(expected, actual);
    expected = INT8_MIN;
    ASSERT_INT8_NOT_EQ(expected, actual);

    actual   = INT8_MIN;
    expected = actual;
    ASSERT_INT8_EQ(expected, actual);
    expected = INT8_MAX;
    ASSERT_INT8_NOT_EQ(expected, actual);

    test_case_end();

    test_case_start("Test int8 asserts, these should fail");

    actual   = INT8_MAX;
    expected = INT8_MIN;
    ASSERT_INT8_EQ(expected, actual);
    expected = actual;
    ASSERT_INT8_NOT_EQ(expected, actual);

    actual   = INT8_MIN;
    expected = INT8_MAX;
    ASSERT_INT8_EQ(expected, actual);
    expected = actual;
    ASSERT_INT8_NOT_EQ(expected, actual);

    test_case_end();
}

/**
 * Test the SimplyC uint8 assertions.
 */
static void test_uint8_asserts (void)
{
    // test the uint8 type, pass and fail
    test_case_start("Test uint8 asserts, these should pass");

    uint8_t actual   = UINT8_MAX;
    uint8_t expected = actual;
    ASSERT_UINT8_EQ(expected, actual);
    expected = 0;
    ASSERT_UINT8_NOT_EQ(expected, actual);

    test_case_end();

    test_case_start("Test uint8 asserts, these should fail");

    actual   = UINT8_MAX;
    expected = 0;
    ASSERT_UINT8_EQ(expected, actual);
    expected = actual;
    ASSERT_UINT8_NOT_EQ(expected, actual);

    test_case_end();
}

/**
 * Test the SimplyC int16 assertions.
 */
static void test_int16_asserts (void)
{
    // test the int16 type, pass and fail
    test_case_start("Test int16 asserts, these should pass");

    int16_t actual   = INT16_MAX;
    int16_t expected = actual;
    ASSERT_INT16_EQ(expected, actual);
    expected = INT16_MIN;
    ASSERT_INT16_NOT_EQ(expected, actual);

    actual   = INT16_MIN;
    expected = actual;
    ASSERT_INT16_EQ(expected, actual);
    expected = INT16_MAX;
    ASSERT_INT16_NOT_EQ(expected, actual);

    test_case_end();

    test_case_start("Test int16 asserts, these should fail");

    actual   = INT16_MAX;
    expected = INT16_MIN;
    ASSERT_INT16_EQ(expected, actual);
    expected = actual;
    ASSERT_INT16_NOT_EQ(expected, actual);

    actual   = INT16_MIN;
    expected = INT16_MAX;
    ASSERT_INT16_EQ(expected, actual);
    expected = actual;
    ASSERT_INT16_NOT_EQ(expected, actual);

    test_case_end();
}

/**
 * Test the SimplyC uint16 assertions.
 */
static void test_uint16_asserts (void)
{
    // test the uint16 type, pass and fail
    test_case_start("Test uint16 asserts, these should pass");

    uint16_t actual   = UINT16_MAX;
    uint16_t expected = actual;
    ASSERT_UINT16_EQ(expected, actual);
    expected = 0;
    ASSERT_UINT16_NOT_EQ(expected, actual);

    test_case_end();

    test_case_start("Test uint16 asserts, these should fail");

    actual   = 0;
    expected = UINT16_MAX;
    ASSERT_UINT16_EQ(expected, actual);
    expected = actual;
    ASSERT_UINT16_NOT_EQ(expected, actual);

    test_case_end();
}

/**
 * Test the SimplyC int32 assertions.
 */
static void test_int32_asserts (void)
{
    // test the int32 type, pass and fail
    test_case_start("Test int32 asserts, these should pass");

    int32_t actual   = INT32_MAX;
    int32_t expected = actual;
    ASSERT_INT32_EQ(expected, actual);
    expected = INT32_MIN;
    ASSERT_INT32_NOT_EQ(expected, actual);

    actual   = INT32_MIN;
    expected = actual;
    ASSERT_INT32_EQ(expected, actual);
    expected = INT32_MAX;
    ASSERT_INT32_NOT_EQ(expected, actual);

    test_case_end();

    test_case_start("Test int32 asserts, these should fail");

    actual   = INT32_MAX;
    expected = INT32_MIN;
    ASSERT_INT32_EQ(expected, actual);
    expected = actual;
    ASSERT_INT32_NOT_EQ(expected, actual);

    actual   = INT32_MIN;
    expected = INT32_MAX;
    ASSERT_INT32_EQ(expected, actual);
    expected = actual;
    ASSERT_INT32_NOT_EQ(expected, actual);

    test_case_end();
}

/**
 * Test the SimplyC uint32 assertions.
 */
static void test_uint32_asserts (void)
{
    // test the uint32 type, pass and fail
    test_case_start("Test uint32 asserts, these should pass");

    uint32_t actual   = UINT32_MAX;
    uint32_t expected = actual;
    ASSERT_UINT32_EQ(expected, actual);
    expected = 0;
    ASSERT_UINT32_NOT_EQ(expected, actual);

    test_case_end();

    test_case_start("Test uint32 asserts, these should fail");

    actual   = UINT32_MAX;
    expected = 0;
    ASSERT_UINT32_EQ(expected, actual);
    expected = actual;
    ASSERT_UINT32_NOT_EQ(expected, actual);

    test_case_end();
}

/**
 * Test the SimplyC int64 assertions.
 */
static void test_int64_asserts (void)
{
    // test the int64 type, pass and fail
    test_case_start("Test int64 asserts, these should pass");

    int64_t actual   = INT64_MAX;
    int64_t expected = actual;
    ASSERT_INT64_EQ(expected, actual);
    expected = INT64_MIN;
    ASSERT_INT64_NOT_EQ(expected, actual);

    actual   = INT64_MIN;
    expected = actual;
    ASSERT_INT64_EQ(expected, actual);
    expected = INT64_MAX;
    ASSERT_INT64_NOT_EQ(expected, actual);

    test_case_end();

    test_case_start("Test int64 asserts, these should fail");

    actual   = INT64_MAX;
    expected = INT64_MIN;
    ASSERT_INT64_EQ(expected, actual);
    expected = actual;
    ASSERT_INT64_NOT_EQ(expected, actual);

    actual   = INT64_MIN;
    expected = INT64_MAX;
    ASSERT_INT64_EQ(expected, actual);
    expected = actual;
    ASSERT_INT64_NOT_EQ(expected, actual);

    test_case_end();
}

/**
 * Test the SimplyC uint64 assertions.
 */
static void test_uint64_asserts (void)
{
    // test the uint64 type, pass and fail
    test_case_start("Test uint64 asserts, these should pass");

    uint64_t actual   = UINT64_MAX;
    uint64_t expected = actual;
    ASSERT_UINT64_EQ(expected, actual);
    expected = 0;
    ASSERT_UINT64_NOT_EQ(expected, actual);

    test_case_end();

    test_case_start("Test uint64 asserts, these should fail");

    actual   = UINT64_MAX;
    expected = 0;
    ASSERT_UINT64_EQ(expected, actual);
    expected = actual;
    ASSERT_UINT64_NOT_EQ(expected, actual);

    test_case_end();
}

/**
 * Test the SimplyC float32 assertions.
 */
static void test_float32_asserts (void)
{
    // test the int64 type, pass and fail
    test_case_start("Test float32 asserts, these should pass");

    float32_t actual   = (float32_t) INT32_MAX;
    float32_t expected = actual;
    ASSERT_FLOAT32_EQ(expected, actual);
    expected = (float32_t) INT32_MIN;
    ASSERT_FLOAT32_NOT_EQ(expected, actual);

    actual   = (float32_t) INT32_MIN;
    expected = actual;
    ASSERT_FLOAT32_EQ(expected, actual);
    expected = (float32_t) INT32_MAX;
    ASSERT_FLOAT32_NOT_EQ(expected, actual);

    test_case_end();

    test_case_start("Test float32 asserts, these should fail");

    actual   = (float32_t) INT32_MAX;
    expected = (float32_t) INT32_MIN;
    ASSERT_FLOAT32_EQ(expected, actual);
    expected = actual;
    ASSERT_FLOAT32_NOT_EQ(expected, actual);

    actual   = (float32_t) INT32_MIN;
    expected = (float32_t) INT32_MAX;
    ASSERT_FLOAT32_EQ(expected, actual);
    expected = actual;
    ASSERT_FLOAT32_NOT_EQ(expected, actual);

    test_case_end();
}

/**
 * Test the SimplyC float64 assertions.
 */
static void test_float64_asserts (void)
{
    // test the int64 type, pass and fail
    test_case_start("Test float64 asserts, these should pass");

    float64_t actual   = (float64_t) INT64_MAX;
    float64_t expected = actual;
    ASSERT_FLOAT64_EQ(expected, actual);
    expected = (float64_t) INT64_MIN;
    ASSERT_FLOAT64_NOT_EQ(expected, actual);

    actual   = (float64_t) INT64_MIN;
    expected = actual;
    ASSERT_FLOAT64_EQ(expected, actual);
    expected = (float64_t) INT64_MAX;
    ASSERT_FLOAT64_NOT_EQ(expected, actual);

    test_case_end();

    test_case_start("Test float64 asserts, these should fail");

    actual   = (float64_t) INT64_MAX;
    expected = (float64_t) INT64_MIN;
    ASSERT_FLOAT64_EQ(expected, actual);
    expected = actual;
    ASSERT_FLOAT64_NOT_EQ(expected, actual);

    actual   = (float64_t) INT64_MIN;
    expected = (float64_t) INT64_MAX;
    ASSERT_FLOAT64_EQ(expected, actual);
    expected = actual;
    ASSERT_FLOAT64_NOT_EQ(expected, actual);

    test_case_end();
}



