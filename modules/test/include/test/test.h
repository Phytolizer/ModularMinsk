#ifndef TEST_TEST_H_
#define TEST_TEST_H_

#include <nonstd/asprintf.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define TEST_ASSERT(Test, Cleanup, ...)                   \
  do {                                                    \
    if (!(Test)) {                                        \
      char* message;                                      \
      NonstdAllocatedStringPrintf(&message, __VA_ARGS__); \
      Cleanup;                                            \
      return message;                                     \
    }                                                     \
  } while (false)

#define TEST_FAIL(...) TEST_ASSERT(false, (void)0, __VA_ARGS__)

#define TEST_RUN_SUITE(Name, Count)            \
  do {                                         \
    char* message = TestSuite##Name(Count);    \
    if (message != NULL) {                     \
      fprintf(stderr, "[FAIL] %s\n", message); \
      free(message);                           \
      exit(EXIT_FAILURE);                      \
    }                                          \
  } while (false)

#define TEST_RUN(Name)                         \
  do {                                         \
    fprintf(stderr, "Running " #Name "...\n"); \
    char* message = Test##Name();              \
    ++(*test_count);                           \
    if (message != NULL) {                     \
      return message;                          \
    }                                          \
  } while (false)

#define TEST_RUN_SUBTEST(Name, Cleanup, ...)        \
  do {                                              \
    char* message = TestSubtest##Name(__VA_ARGS__); \
    if (message != NULL) {                          \
      Cleanup;                                      \
      return message;                               \
    }                                               \
  } while (false)

#define TEST_SUBTEST_FUNC(Name, ...) char* TestSubtest##Name(__VA_ARGS__)
#define TEST_FUNC(Name) char* Test##Name(void)
#define TEST_SUITE_FUNC(Name) char* TestSuite##Name(uint64_t* test_count)
#define TEST_PASS() return NULL
#define TEST_SUITE_PASS() return NULL

#endif  // TEST_TEST_H_
