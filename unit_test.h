/**
 * @file unit_test.h
 * 
 * @mainpage SimplyC Unit Test Framework
 * SimplyC is a unit test framework optimized for running unit tests in
 * an embedded environment. Often these environments have limited memory
 * and little support for standard I/O functions. This framework can be
 * tailored to accommodate these environments. 
 * 
 * Your test framework should not be the source of problems in your testing
 * environment. To this end, the development of the SimplyC framework 
 * followed basic, good development practices:
 * - Embedded C Coding Standard was followed (Barr Group)
 * - lint analysis was performed using PC-Lint (Gimpel Software)
 * - cyclomatic complexity and code quality metrics were performed using 
 *   Resource Standard Metrics (M Squared Technologies)
 *
 *    ### Overview
 * 
 *    This framework is built around the idea of test suites, test cases and
 *    assertions. In general, a test suite can be viewed as testing a single
 *    C source file. A test suite contains multiple test cases. Each test case
 *    can be viewed as testing an individual C function. Within each test case,
 *    assertions are used to compare expected results to actual results. The
 *    output of the unit tests is recorded to standard out and to a log file
 *    which can be analyzed after the unit tests are executed.
 *
 *    ### Example
 *
 *    Consider a file named packet_builder.c. This file contains a function
 *    that qualifies to be unit tested (it is well encapsulated and loosely
 *    coupled). This function is named config_frame_build() and it accesses
 *    the following data structure, also in the file:
 *
 *        static struct
 *        {
 *          uint16_t header_id;               
 *          uint8_t  node_num;
 *          uint32_t data;
 *          bool     config_valid;
 *        } config_frame;
 *
 *    packet_builder.c contains a function called packet_builder_unit_tests()
 *    Most likely, the unit test functions are conditionally compiled in
 *    the file.
 *
 *        #ifdef UNIT_TESTS
 *
 *        void packet_builder_unit_tests (void)
 *        {
 *          test_suite_start("Packet Builder Test Suite");
 *
 *          // functions can be created that hold the test cases, call as many
 *          // as you like, there is no limit to the number of test cases in a
 *          // test suite
 *          test_config_frame_build();
 *
 *          test_suite_end();
 *        }
 *
 *        static void test_config_frame_build (void)
 *        {
 *          test_case_start("Verify config frame correctly built");
 *
 *          // call any functions in the code that are used to initialize
 *          // the configuration frames, use legal values for this
 *          prep_high_speed_config();
 *          config_node_set(HIGH_NODE_NUM);
 *          config_data_set(0x12345678);
 *
 *          // call the function we are testing
 *          config_frame_build();
 *
 *          // we know from the design that the data structure, config_frame, 
 *          // should contain specific values after the function is called, 
 *          // verify that these are correct using the unit test asserts
 *
 *          ASSERT_UINT16_EQ(FAST_CONFIG_HEADER, config_frame.header_id);
 *          ASSERT_UINT8_EQ (HIGH_NODE_NUM,      config_frame.node_num);
 *          ASSERT_UINT32_EQ(0x12345678,         config_frame.data);
 *          ASSERT_BOOL_EQ  (true,               config_frame.config_valid);
 * 
 *          test_case_end();
 * 
 *          test_case_start("Verify config frame limits enforced");
 *
 *          // use an illegal value and make sure it is rejected
 *          config_node_set(OUT_OF_BOUNDS_NODE_NUM);
 *
 *          // call the function we are testing
 *          config_frame_build();
 *
 *          // this configuration should be rejected
 *          ASSERT_BOOL_EQ(false, config_frame.config_valid);
 * 
 *          test_case_end();
 *
 *          // continue on with as many test cases as necessary to fully
 *          // exercise the config_frame_build function
 *        }
 *
 *        #endif // UNIT_TESTS
 *
 *    More than likely, there will be a main function somewhere that
 *    calls all the unit tests in a project:
 *
 *        void unit_tests_run (void)
 *        {
 *          // turn logging on for the unit test framework, this opens a
 *          // file for logging
 *          unit_test_log_on("unit_test_log.txt");
 *
 *          // call all the unit test suites you want
 *          packet_builder_unit_tests();
 *
 *          // clean up and close the logging file
 *          unit_test_log_off();
 *        }
 *
 *    ### Assumptions
 *
 *    - Unit testing is performed in a single thread.
 *    - Only one test suite is executed at a time.
 *    - Only one test case is executed at a time.
 *
 *    ### System Requirements
 * 
 *    It is assumed that the development environment being used provides
 *    fixed-width data types and a boolean data type via the following
 *    files:
 * 
 *    - stdint.h
 *    - stdbool.h
 * 
 *    If these files are not included in your development environment, it is
 *    a good idea to create your own. Determine the width of the integer types
 *    supported by your processor/compiler and create typedefs for each using
 *    the names introduced by the C99 Standard.
 * 
 *    It is assumed that the following library functions are provided by the
 *    the environment. If these are not available, the logging functions can be
 *    modified to support the target environment.
 *
 *    - snprintf
 *    - printf
 *    - fprintf
 *
 *    ### Error Checking
 * 
 *    Error checking in the framework is limited to verifying that test suites
 *    and test cases are only called one at a time. Any other mis-use of the
 *    framework is allowed:)
 * 
 * @author Susan McCord
 * 
 * @version 1.00
 * 
 * @copyright This program is free software. You can redistribute it and/or 
 * modify it under the terms of the GNU General Public License, version 3 
 * (GPLv3).
 */
#ifndef _UNIT_TEST_H_
#define _UNIT_TEST_H_

#ifdef __cplusplus
extern "C" {
#endif

//RSM_IGNORE_BEGIN allow line to exceed 80 characters for easy viewing of macros

/**
 * Using conditional compilation, the unit test framework can be configured to 
 * exclude support for logging, 64-bit integer and floating point numbers.
 * Define the following constants in your code or environment to include/exclude
 * support for these.
 * 
 * By default, support for logging is on and support for 64-bit integer and
 * floating point is configured off.
 * 
 * - UNIT_TEST_LOG
 * - UNIT_TEST_INT64
 * - UNIT_TEST_FLOATING_POINT
 */
#define UNIT_TEST_LOG  1

/**
 * Declarations for unit test framework functions.
 */
extern void unit_test_log_on (char const *);
extern void unit_test_log_off(void);
extern void test_suite_start (char const *);
extern void test_suite_end   (void);
extern void test_case_start  (char const *);
extern void test_case_end    (void);

/**
 * Use these asserts to verify test results. Call the assert functions directly
 * or use the provided macros.
 */
#define ASSERT_BOOL_EQ(e,a)     (assert_bool_eq    (e, a, __FILE__, __LINE__))
#define ASSERT_BOOL_NOT_EQ(e,a) (assert_bool_not_eq(e, a, __FILE__, __LINE__))
extern void assert_bool_eq    (bool, bool, char const *, int);
extern void assert_bool_not_eq(bool, bool, char const *, int);

#define ASSERT_INT8_EQ(e,a)      (assert_int8_eq     (e, a, __FILE__, __LINE__))
#define ASSERT_INT8_NOT_EQ(e,a)  (assert_int8_not_eq (e, a, __FILE__, __LINE__))
#define ASSERT_UINT8_EQ(e,a)     (assert_uint8_eq    (e, a, __FILE__, __LINE__))
#define ASSERT_UINT8_NOT_EQ(e,a) (assert_uint8_not_eq(e, a, __FILE__, __LINE__))
extern void assert_int8_eq     (int8_t,  int8_t,  char const *, int);
extern void assert_int8_not_eq (int8_t,  int8_t,  char const *, int);
extern void assert_uint8_eq    (uint8_t, uint8_t, char const *, int);
extern void assert_uint8_not_eq(uint8_t, uint8_t, char const *, int);

#define ASSERT_INT16_EQ(e,a)      (assert_int16_eq     (e, a, __FILE__, __LINE__))
#define ASSERT_INT16_NOT_EQ(e,a)  (assert_int16_not_eq (e, a, __FILE__, __LINE__))
#define ASSERT_UINT16_EQ(e,a)     (assert_uint16_eq    (e, a, __FILE__, __LINE__))
#define ASSERT_UINT16_NOT_EQ(e,a) (assert_uint16_not_eq(e, a, __FILE__, __LINE__))
extern void assert_int16_eq     (int16_t,  int16_t,  char const *, int);
extern void assert_int16_not_eq (int16_t,  int16_t,  char const *, int);
extern void assert_uint16_eq    (uint16_t, uint16_t, char const *, int);
extern void assert_uint16_not_eq(uint16_t, uint16_t, char const *, int);

#define ASSERT_INT32_EQ(e,a)      (assert_int32_eq     (e, a, __FILE__, __LINE__))
#define ASSERT_INT32_NOT_EQ(e,a)  (assert_int32_not_eq (e, a, __FILE__, __LINE__))
#define ASSERT_UINT32_EQ(e,a)     (assert_uint32_eq    (e, a, __FILE__, __LINE__))
#define ASSERT_UINT32_NOT_EQ(e,a) (assert_uint32_not_eq(e, a, __FILE__, __LINE__))
extern void assert_int32_eq     (int32_t,  int32_t,  char const *, int);
extern void assert_int32_not_eq (int32_t,  int32_t,  char const *, int);
extern void assert_uint32_eq    (uint32_t, uint32_t, char const *, int);
extern void assert_uint32_not_eq(uint32_t, uint32_t, char const *, int);

/**
 *  64-bit integer support
 */
#ifdef UNIT_TEST_INT64

#define ASSERT_INT64_EQ(e,a)      (assert_int64_eq     (e, a, __FILE__, __LINE__))
#define ASSERT_INT64_NOT_EQ(e,a)  (assert_int64_not_eq (e, a, __FILE__, __LINE__))
#define ASSERT_UINT64_EQ(e,a)     (assert_uint64_eq    (e, a, __FILE__, __LINE__))
#define ASSERT_UINT64_NOT_EQ(e,a) (assert_uint64_not_eq(e, a, __FILE__, __LINE__))
extern void assert_int64_eq     (int64_t,  int64_t,  char const *, int);
extern void assert_int64_not_eq (int64_t,  int64_t,  char const *, int);
extern void assert_uint64_eq    (uint64_t, uint64_t, char const *, int);
extern void assert_uint64_not_eq(uint64_t, uint64_t, char const *, int);

#endif

/**
 * Floating point number support, provide typedefs for floating point types if 
 * they are not provided by the environment.
 * 
 *  typedef float  float32_t;
 *  typedef double float64_t;
 */
#ifdef UNIT_TEST_FLOATING_POINT
typedef float  float32_t;
typedef double float64_t;

/**
 * Set the relative error used to compare floats, this determines how close
 * floating point numbers need to be to each other in order to be considered
 * equal. This value can be changed to suit the application.
 */
#define MAX_FLOAT_RELATIVE_ERROR ((float64_t) 1.0e-5) // 99.999% accuracy

/**
 * Set the absolute error, this is used when floats are very close to
 * zero, but of different signs. This value can be changed to suit the
 * application.
 */
#define MAX_FLOAT_ABSOLUTE_ERROR ((float64_t) 1.0e-37)

#define ASSERT_FLOAT32_EQ(e,a)     (assert_float32_eq     (e, a, __FILE__, __LINE__))
#define ASSERT_FLOAT32_NOT_EQ(e,a) (assert_float32_not_eq (e, a, __FILE__, __LINE__))
#define ASSERT_FLOAT64_EQ(e,a)     (assert_float64_eq     (e, a, __FILE__, __LINE__))
#define ASSERT_FLOAT64_NOT_EQ(e,a) (assert_float64_not_eq (e, a, __FILE__, __LINE__))
extern void assert_float32_eq    (float32_t, float32_t, char const *, int);
extern void assert_float32_not_eq(float32_t, float32_t, char const *, int);
extern void assert_float64_eq    (float64_t, float64_t, char const *, int);
extern void assert_float64_not_eq(float64_t, float64_t, char const *, int);

#endif

//RSM_IGNORE_END

#ifdef __cplusplus
}
#endif

#endif



