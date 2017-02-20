/**
 * @file unit_test.c
 *
 * @brief These functions provide a framework for unit testing in C. A test
 * suite can be viewed as testing a C source file. A test case can be viewed
 * as testing a single C function. For an example of how to use the API of 
 * the framework, see the comments in unit_test.h.
 *
 * @copyright This program is free software. You can redistribute it and/or 
 * modify it under the terms of the GNU General Public License, version 3 
 * (GPLv3).
 */
#include <stdbool.h>       // allow the use of boolean data type
#include <stdint.h>        // standard fixed-width data types
#include "unit_test.h"     // common declarations for the unit test project

#ifdef UNIT_TEST_FLOATING_POINT
#include <math.h>          // to provide fabs
#endif

#ifdef UNIT_TEST_LOG
//! Unit test results can be logged to a file
#include <stdio.h>         // to provide printf/fprintf/snprintf
static FILE *log_file = 0;
#endif

//! Maximum length of an error message
#define MAX_MSG_LEN ((int) 100)

//! Buffer used to create assertion failure messages
static char error_msg[MAX_MSG_LEN + 1];

//! Macro to allow creation of assertion failure messages when 2 values
//! should be equal. By default, this uses snprintf. Change this to suit
//! your environment if needed.\n
//!
//! msg: char buffer used to hold the message    \n
//! s:   format string                           \n
//! e:   expected value                          \n
//! a:   actual value                            \n
#ifdef UNIT_TEST_LOG
#define EQ_ERR_MSG_CREATE(msg, s, e, a) \
    (void) snprintf(msg, MAX_MSG_LEN, s, e, a)
#else
#define EQ_ERR_MSG_CREATE(msg, s, e, a) ((void) 0)
#endif
    
//! Macro to allow creation of assertion failure messages when a value is equal
//! to something that is not expected. By default, this uses snprintf. Change
//! this to suit your environment if needed.\n
//!
//! msg: char buffer used to hold the message
//! s:   format string                           \n
//! e:   value that is not expected              \n
#ifdef UNIT_TEST_LOG
#define NOT_EQ_ERR_MSG_CREATE(msg, s, e) \
    (void) snprintf(msg, MAX_MSG_LEN, s, e)
#else
#define NOT_EQ_ERR_MSG_CREATE(msg, s, e) ((void) 0)
#endif

//! true   if a test suite is currently executing
//! false  if a test suite is currently NOT executing
static bool test_suite_active = false;

//! true   if a test case is currently executing
//! false  if a test case is currently NOT executing
static bool test_case_active = false;

//! true   if the current test case passed all assertions
//! false  if at least one test case assertion failed
static bool current_test_case_pass = true;

//! if any asserts fail during a run, this is set to true
static bool failed_assert = false;

// static function declarations
static void log_msg(char const *);
static void log_msg_num(char const *, uint16_t const);
static void log_msg_str(char const *, char const *);
static void log_assert_fail(char const *, char const *, 
    int const, char const *);
static void assert_failed(char const *, int, char const *);

#ifdef UNIT_TEST_FLOATING_POINT
static bool float64_eq(float64_t, float64_t);
#endif

/**
 *  Call this function to indicate the start of a new test suite. A test
 *  suite can be viewed as unit testing a C source file.
 *
 *  @attention - Remember to call test_suite_end when a test suite is complete.
 *
 *  @param test_suite_name string containing the name of the test suite.
 *  In most cases, this would contain the name of the file being unit
 *  tested.
 */
void test_suite_start (char const *test_suite_name)
{
    static uint16_t test_suite_num = 0;

    if (!test_suite_active)
    {
        /*
         * Create a unique number for each test suite. This is to make it
         * easier to refer to the output when analyzing the results.
         */
        log_msg_num("\n\nTest Suite Number: %d", ++test_suite_num);
        log_msg_str("\nTest Suite Name: %s", test_suite_name);

        test_suite_active = true;

        // clear the error message buffer
        for(uint16_t index = 0; index < sizeof(error_msg); index++)
        {
            error_msg[index] = 0;
        }
    }
    else
    {
        // A test suite is already active: Bad use of API!
        log_msg("\n\nERROR: A test suite is already active.");
        log_msg_str("\nCannot execute \"%s\"", test_suite_name);
        log_msg("\nOnly one test suite can be executed at a time.\n");
    }
}

/**
 *  Call this to indicate a test suite has completed. The framework checks to
 *  see if a test suite is active and logs the appropriate message.
 */
void test_suite_end (void)
{
    if (test_suite_active)
    {
        log_msg("\n\nTest Suite Complete\n");
        test_suite_active = false;
    }
    else
    {
        // A test suite is not active: Bad use of API!
        log_msg("\n\nERROR: A test suite is not active.");
        log_msg("\nCall 'test_suite_start' first.\n");
    }
}

/**
 *  Call this to indicate the start of a new test case. If any of the asserts
 *  fail for a test case, then the entire test case fails.
 *
 *  @attention - Remember to call test_case_end when you are done with a
 *  test case.
 *
 *  @param test_case_name - string containing the name of the test case.
 *  In most cases this would contain the name of the function being tested.
 */
void test_case_start (char const *test_case_name)
{
    if (!test_case_active)
    {
        log_msg_str("\n\nTest Case: %s", test_case_name);

        // reset the flag indicating the test case result
        current_test_case_pass = true;

        // set the flag indicating that a test case is active
        test_case_active = true;
    }
    else
    {
        // A test case is already active: Bad use of API!
        log_msg("\n\nERROR: A test case is already active.");
        log_msg_str("\nCannot execute \"%s\"", test_case_name);
        log_msg("\nOnly one test case can be executed at a time.\n");
    }
}

/**
 *  Call this to indicate a test case has completed. The framework checks that
 *  a test case passed all assertions and logs the appropriate message.
 */
void test_case_end (void)
{
    if (test_case_active)
    {
        if (current_test_case_pass)
        {
            log_msg("\nTest Case Passed");
        }
        else
        {
            log_msg("\nTest Case Failed");
        }

        test_case_active = false;
    }
    else
    {
        // A test case is not active: Bad use of API!
        log_msg("\n\nERROR: A test case is not active.");
        log_msg("\nCall the 'test_case_start' function first.\n");
    }
}

/**
 * @return true if all tests have passed in a run, false otherwise.
 */
bool unit_test_all_success (void)
{
    return !failed_assert;
}

/**
 *  Asserts if the bool values ARE NOT equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 */
void assert_bool_eq (bool expected, bool actual,
        char const *file, int line_num)
{
    if (expected != actual)
    {
        // create an error message with details
        EQ_ERR_MSG_CREATE(error_msg, " expected: %u, got: %u",
            expected, actual);

        assert_failed(file, line_num, error_msg);
    }
}

/**
 *  Asserts if the bool values ARE equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 */
void assert_bool_not_eq (bool expected, bool actual,
        char const *file, int line_num)
{
   if (expected == actual)
    {
        // create an error message with details
        NOT_EQ_ERR_MSG_CREATE(error_msg, " should not be: %u", expected);

        assert_failed(file, line_num, error_msg);
    }
}

/**
 *  Asserts if the int8_t ARE NOT equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 *
 */
void assert_int8_eq (int8_t expected, int8_t actual,
        char const *file, int line_num)
{
    if (expected != actual)
    {
        // create an error message with details
        EQ_ERR_MSG_CREATE(error_msg, " expected: %d, got: %d",
            expected, actual);

        assert_failed(file, line_num, error_msg);
    }
}

/**
 *  Asserts if the int8_t ARE equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 */
void assert_int8_not_eq (int8_t expected, int8_t actual,
        char const *file, int line_num)
{
    if (expected == actual)
    {
        // create an error message with details
        NOT_EQ_ERR_MSG_CREATE(error_msg, " should not be: %d", expected);

        assert_failed(file, line_num, error_msg);
    }
}

/**
 *  Asserts if the uint8_t ARE NOT equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 */
void assert_uint8_eq (uint8_t expected, uint8_t actual,
        char const *file, int line_num)
{
    if (expected != actual)
    {
        // create an error message with details
        EQ_ERR_MSG_CREATE(error_msg, " expected: %u, got: %u",
                expected, actual);

        assert_failed(file, line_num, error_msg);
    }
}

/**
 *  Asserts if the uint8_t ARE equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 */
void assert_uint8_not_eq (uint8_t expected, uint8_t actual,
        char const *file, int line_num)
{
    if (expected == actual)
    {
        // create an error message with details
        NOT_EQ_ERR_MSG_CREATE(error_msg, " should not be: %u", expected);

        assert_failed(file, line_num, error_msg);
    }
}

/**
 *  Asserts if the int16_t ARE NOT equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 */
void assert_int16_eq (int16_t expected, int16_t actual,
        char const *file, int line_num)
{
    if (expected != actual)
    {
        // create an error message with details
        EQ_ERR_MSG_CREATE(error_msg, " expected: %d, got: %d",
                expected, actual);

        assert_failed(file, line_num, error_msg);
    }
}

/**
 *  Asserts if the int16_t ARE equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 */
void assert_int16_not_eq (int16_t expected, int16_t actual,
        char const *file, int line_num)
{
    if (expected == actual)
    {
        // create an error message with details
        NOT_EQ_ERR_MSG_CREATE(error_msg, " should not be: %d", expected);

        assert_failed(file, line_num, error_msg);
    }
}

/**
 *  Asserts if the uint16_t ARE NOT equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 */
void assert_uint16_eq (uint16_t expected, uint16_t actual,
        char const *file, int line_num)
{
    if (expected != actual)
    {
        // create an error message with details
        EQ_ERR_MSG_CREATE(error_msg, " expected: %u, got: %u",
                expected, actual);

        assert_failed(file, line_num, error_msg);
    }
}

/**
 *  Asserts if the uint16_t ARE equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 */
void assert_uint16_not_eq (uint16_t expected, uint16_t actual,
        char const *file, int line_num)
{
    if (expected == actual)
    {
        // create an error message with details
        NOT_EQ_ERR_MSG_CREATE(error_msg, " should not be: %u", expected);

        assert_failed(file, line_num, error_msg);
    }
}

/**
 *  Asserts if the int32_t ARE NOT equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 *
 */
void assert_int32_eq (int32_t expected, int32_t actual,
        char const *file, int line_num)
{
    if (expected != actual)
    {
        // create error message with details
        EQ_ERR_MSG_CREATE(error_msg, " expected: %d, got: %d",
                expected, actual);

        assert_failed(file, line_num, error_msg);
    }
}

/**
 *  Asserts if the int32_t ARE equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 *
 */
void assert_int32_not_eq (int32_t expected, int32_t actual,
        char const *file, int line_num)
{
    if (expected == actual)
    {
        // create an error message with details
        NOT_EQ_ERR_MSG_CREATE(error_msg, " should not be: %d", expected);

        assert_failed(file, line_num, error_msg);
    }
}

/**
 *  Asserts if the uint32_t ARE NOT equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 */
void assert_uint32_eq (uint32_t expected, uint32_t actual,
        char const *file, int line_num)
{
    if (expected != actual)
    {
        // create error message with details
        EQ_ERR_MSG_CREATE(error_msg, " expected: %u, got: %u",
                expected, actual);

        assert_failed(file, line_num, error_msg);
    }
}

/**
 *  Asserts if the uint32_t ARE equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 */
void assert_uint32_not_eq (uint32_t expected, uint32_t actual,
        char const *file, int line_num)
{
    if (expected == actual)
    {
        // create an error message with details
        NOT_EQ_ERR_MSG_CREATE(error_msg, " should not be: %u", expected);

        assert_failed(file, line_num, error_msg);
    }
}

#ifdef UNIT_TEST_INT64

/**
 *  Asserts if the int64_t ARE NOT equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 */
void assert_int64_eq (int64_t expected, int64_t actual,
        char const *file, int line_num)
{
    if (expected != actual)
    {
        // create error message with details
        EQ_ERR_MSG_CREATE(error_msg, " expected: %lld, got: %lld",
                expected, actual);

        assert_failed(file, line_num, error_msg);
    }
}

/**
 *  Asserts if the int64_t ARE equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 */
void assert_int64_not_eq (int64_t expected, int64_t actual,
        char const *file, int line_num)
{
    if (expected == actual)
    {
        // create an error message with details
        NOT_EQ_ERR_MSG_CREATE(error_msg, " should not be: %lld", expected);

        assert_failed(file, line_num, error_msg);
    }
}

/**
 *  Asserts if the uint64_t ARE NOT equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 */
void assert_uint64_eq (uint64_t expected, uint64_t actual,
        char const *file, int line_num)
{
    if (expected != actual)
    {
        // create error message with details
        EQ_ERR_MSG_CREATE(error_msg, " expected: %llu, got: %llu",
                expected, actual);

        assert_failed(file, line_num, error_msg);
    }
}

/**
 *  Asserts if the uint64_t ARE equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 */
void assert_uint64_not_eq (uint64_t expected, uint64_t actual,
        char const *file, int line_num)
{
    if (expected == actual)
    {
        // create an error message with details
        NOT_EQ_ERR_MSG_CREATE(error_msg, " should not be: %llu", expected);

        assert_failed(file, line_num, error_msg);
    }
}

#endif

#ifdef UNIT_TEST_FLOATING_POINT

/**
 *  Asserts if the float32_t ARE NOT equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 */
void assert_float32_eq (float32_t expected, float32_t actual,
        char const *file, int line_num)
{
    assert_float64_eq((float64_t) expected, (float64_t) actual,
            file, line_num);
}

/**
 *  Asserts if the float32_t ARE equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 */
void assert_float32_not_eq (float32_t expected, float32_t actual,
        char const *file, int line_num)
{
    assert_float64_not_eq((float64_t) expected, (float64_t) actual,
            file, line_num);
}

/**
 *  Asserts if the float64_t ARE NOT equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 */
void assert_float64_eq (float64_t expected, float64_t actual,
        char const *file, int line_num)
{
    bool equal = float64_eq(expected, actual);

    if (!equal)
    {
        // create an error message with details
        EQ_ERR_MSG_CREATE(error_msg, " expected: %e, got: %e",
                expected, actual);

        assert_failed(file, line_num, error_msg);
    }
}

/**
 *  Asserts if the float64_t ARE equal.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 *  @param file       the source file name
 *  @param line_num   the source code line number
 */
void assert_float64_not_eq (float64_t expected, float64_t actual,
        char const *file, int line_num)
{
    bool equal = float64_eq(expected, actual);

    if (equal)
    {
        // create an error message with details
        NOT_EQ_ERR_MSG_CREATE(error_msg, " should not be: %e", expected);

        assert_failed(file, line_num, error_msg);
    }
}

/**
 *  Compare 2 floating point numbers for equality.
 *
 *  @param expected   the expected value
 *  @param actual     the value to compare to
 * 
 *  @return true if the values are equal, false otherwise
 */
//lint -e{777} allow explicit comparison to 0
static bool float64_eq (float64_t expected, float64_t actual)
{
    float64_t relative_error;
    bool equal = false;

    // do explicit checks for zero to avoid any divide-by-zero issues
    if ((float64_t) 0.0 == expected)
    {
        equal = (fabs(actual) < MAX_FLOAT_ABSOLUTE_ERROR)
                ? true : false;
    }
    else if ((float64_t) 0.0 == actual)
    {
        equal = (fabs(expected) < MAX_FLOAT_ABSOLUTE_ERROR)
                ? true : false;
    }
    else if (fabs(expected - actual) < MAX_FLOAT_ABSOLUTE_ERROR)
    {
        // see if they are close enough in absolute terms
        equal = true;
    }
    else
    {
        // see if they are close enough in relative terms,
        if (fabs(expected) > fabs(actual))
        {
            relative_error = fabs((expected - actual) / expected);
        }
        else
        {
            relative_error = fabs((actual - expected) / actual);
        }

        if (relative_error < MAX_FLOAT_RELATIVE_ERROR)
        {
            equal = true;
        }
    }

    return equal;
}

#endif // UNIT_TEST_FLOATING_POINT

/**
 *  Open the unit test log file. This function should be called prior to
 *  any test suites being called.
 *
 *  @param  file_name name of the file to open or null if no file is desired
 */
void unit_test_log_on (char const *file_name)
{
    #ifdef UNIT_TEST_LOG
    if (file_name)
    {
        log_file = fopen(file_name, "w");
        
        // track whether there are any failed asserts during the run
        failed_assert = false;
    }
    #endif
}

/**
 *  Close the unit test log file. This should be called only after all the test
 *  suites have been executed.
 */
void unit_test_log_off (void)
{
    #ifdef UNIT_TEST_LOG
    if (log_file)
    {
        (void) fclose(log_file);
        log_file = 0;
    }
    #endif
}

/**
 *  This function is called when an assertion fails. An error message is
 *  logged and test case failure is noted.
 *
 *  @param file     the source file name
 *  @param line_num the source code line number
 *  @param msg      the message to be displayed
 */
static void assert_failed (char const *file, int line_num, char const *msg)
{
    log_assert_fail("\n    Assert Failed in File: %s, Line %d: %s",
            file, line_num, msg);

    // the current test case has failed
    current_test_case_pass = false;
    
    // if any unit test case fails during a run, this is set to true
    failed_assert = true;
}

/**
 * If logging is turned on, log a message to stdout and to the log file.
 *
 * @param[in] msg_str string to print
 */
//lint -e{592} non-literal format specifier
static void log_msg (char const *msg_str)
{
    #ifdef UNIT_TEST_LOG
    if (log_file)
    {
         // print to standard output and write to file
        (void) printf(msg_str);
        (void) fprintf(log_file, msg_str);
    }
    #endif
}

/**
 * If logging is turned on, log a message and single number to stdout and to
 * the log file. It is assumed that the message string contains a single
 * format specifier for a integer.
 *
 *  @param[in] format_str  format string
 *  @param[in] num         number to print
 */
static void log_msg_num (char const *format_str, uint16_t const num)
{
    #ifdef UNIT_TEST_LOG
    if(log_file)
    {
        // print to standard output and write to file
        (void) printf(format_str, num);
        (void) fprintf(log_file, format_str, num);
    }
    #endif
}

/**
 * If logging is turned on, log a message and single number to stdout and to
 * the log file. It is assumed that the message string contains a single
 * format specifier for a string.
 *
 *  @param[in] format_str  format string
 *  @param[in] str         string to print
 */
static void log_msg_str (char const *format_str, char const *str)
{
    #ifdef UNIT_TEST_LOG
    if(log_file)
    {
        // print to standard output and write to file
        (void) printf(format_str, str);
        (void) fprintf(log_file, format_str, str);
    }
    #endif
}

/**
 * If logging is turned on, log a message, file name and line number to stdout
 * and the log file.
 *
 *  @param[in] format_str  format string
 *  @param[in] file_name   name of file
 *  @param[in] line_num    line number
 *  @param[in] msg_str     message to print
 */
static void log_assert_fail (char const *format_str,
    char const *file_name, int const line_num, char const *msg_str)
{
    #ifdef UNIT_TEST_LOG
    if(log_file)
    {
        // print to standard output and write to file
        (void) printf(format_str, file_name, line_num, msg_str);
        (void) fprintf(log_file, format_str, file_name, line_num, msg_str);
    }
    #endif
}


