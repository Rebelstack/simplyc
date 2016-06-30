# SimplyC Version 1.00
SimplyC is a unit test framework optimized for running unit tests in  an embedded environment. It consists of 2 files: unit_test.c/h. There are 2 additional files provided in the repository: simplyc_test.c/h which consist of test code that verifies the functionality of the SimplyC framework.
## Getting Started
Review the documentation in unit_test.h and review the test code in simplyc_test.c for an overview of how to use the SimplyC framework.
## Customize
SimplyC provides 3 ways to tailor the framework for your environment using conditional compilation.
- UNIT_TEST_LOG: SimplyC uses several standard I/O functions (snprintf, printf, fprintf) to log the results of the unit tests. If your environment provides these functions, define the constant UNIT_TEST_LOG. If your environment does not provide these functions, or the stdio library consumes too much memory, either disable logging by leaving UNIT_TEST_LOG undefined or modify the log functions to suit your needs.
- UNIT_TEST_INT64: If your environment supports 64-bit integers and you need the unit tests to support this, define the constant UNIT_TEST_INT64.
- UNIT_TEST_FLOATING_POINT: If your environment supports floating point numbers and you need the unit tests to support this, define the constant UNIT_TEST_FLOATING_POINT. If you do need floating point support, review the constants MAX_FLOAT_RELATIVE_ERROR and MAX_FLOAT_ABSOLUTE_ERROR and make sure they are appropriate for your environment.


