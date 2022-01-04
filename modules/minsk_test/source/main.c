#include <ansi_esc/ansi_esc.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <test/test.h>

#include "minsk_test/code_analysis/syntax/lexer_tests.h"

void RunAllTests(TestState* test_state) {
  TEST_RUN_SUITE(LexerTests, test_state);
}

int main(void) {
  TestState test_state = {0};
  RunAllTests(&test_state);
  printf("==============\n");
  printf(" TEST SUMMARY \n");
  printf("==============\n");
  printf("%" PRIu64 " tests " ANSI_ESC_FG_GREEN "passed" ANSI_ESC_RESET "\n",
         test_state.tests_passed);
  printf("%" PRIu64 " tests " ANSI_ESC_FG_RED "failed" ANSI_ESC_RESET "\n",
         test_state.tests_failed);
  printf("%" PRIu64 " assertions made" ANSI_ESC_RESET "\n",
         test_state.assert_count);

  return test_state.tests_failed != 0;
}
