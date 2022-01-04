#include <ansi_esc/ansi_esc.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <test/test.h>

#include "minsk_test/code_analysis/syntax/lexer_tests.h"

char* RunAllTests(uint64_t* test_count) {
  TEST_RUN_SUITE(LexerTests, test_count);
  return NULL;
}

int main(void) {
  uint64_t test_count = 0;
  char* message = RunAllTests(&test_count);
  printf("%" PRIu64 " tests passed\n", test_count);
  if (message != NULL) {
    fprintf(stderr, ANSI_ESC_FG_RED "FAIL" ANSI_ESC_RESET ": %s\n", message);
    free(message);
    return 1;
  }
  return 0;
}
