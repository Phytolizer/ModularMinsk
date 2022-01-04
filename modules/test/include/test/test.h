#ifndef TEST_TEST_H_
#define TEST_TEST_H_

#include <ansi_esc/ansi_esc.h>
#include <nonstd/asprintf.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  uint64_t tests_passed;
  uint64_t tests_failed;
  uint64_t assert_count;
} TestState;

#define TEST_ASSERT(Test, Cleanup, ...)                   \
  do {                                                    \
    ++test_state->assert_count;                           \
    if (!(Test)) {                                        \
      char* message;                                      \
      NonstdAllocatedStringPrintf(&message, __VA_ARGS__); \
      Cleanup;                                            \
      return message;                                     \
    }                                                     \
  } while (false)

#define TEST_FAIL(...) TEST_ASSERT(false, (void)0, __VA_ARGS__)

#define TEST_RUN_SUITE(Name, State)       \
  do {                                    \
    fprintf(stderr, "SUITE " #Name "\n"); \
    TestSuite##Name(State);               \
  } while (false)

#define TEST_RUN(Name)                                                \
  do {                                                                \
    fprintf(stderr, " TEST " #Name "\n");                             \
    char* message = Test##Name(test_state);                           \
    if (message != NULL) {                                            \
      ++test_state->tests_failed;                                     \
      fprintf(stderr, ANSI_ESC_FG_RED "FAIL" ANSI_ESC_RESET ": %s\n", \
              message);                                               \
      free(message);                                                  \
    }                                                                 \
    ++test_state->tests_passed;                                       \
  } while (false)

#define TEST_RUN_SUBTEST(Name, Cleanup, ...)        \
  do {                                              \
    char* message = TestSubtest##Name(__VA_ARGS__); \
    if (message != NULL) {                          \
      Cleanup;                                      \
      return message;                               \
    }                                               \
  } while (false)

#define TEST_SUITE_FUNC(Name) void TestSuite##Name(TestState* test_state)
#define TEST_FUNC(Name) char* Test##Name(TestState* test_state)
#define TEST_SUBTEST_FUNC(Name, ...) char* TestSubtest##Name(__VA_ARGS__)
#define TEST_SUITE_PASS() return NULL
#define TEST_PASS() return NULL
#define TEST_SUBTEST_PASS() return NULL

#endif  // TEST_TEST_H_
