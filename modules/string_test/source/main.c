#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <string/string.h>
#include <test/test.h>

TEST_SUBTEST_FUNC(CheckVsAtof, const char* cstr) {
  float a = atof(cstr);
  StringConversionResultF32 b = StringViewToF32(StringViewFromC(cstr));
  TEST_ASSERT(b.success, (void)0, "Failed to convert '%s'", cstr);
  TEST_ASSERT(a == b.value, (void)0, "Expected '%s' to be %f, but got %f", cstr,
              a, b.value);
  TEST_PASS();
}

TEST_FUNC(LeadingDot) {
  TEST_RUN_SUBTEST(CheckVsAtof, (void)0, ".1");
  TEST_PASS();
}

TEST_FUNC(Simple) {
  TEST_RUN_SUBTEST(CheckVsAtof, (void)0, "1");
  TEST_PASS();
}

TEST_FUNC(DecimalPoint) {
  TEST_RUN_SUBTEST(CheckVsAtof, (void)0, "1.1");
  TEST_PASS();
}

TEST_FUNC(Exponent) {
  TEST_RUN_SUBTEST(CheckVsAtof, (void)0, "1.1e2");
  TEST_PASS();
}

TEST_FUNC(ExponentWithSign) {
  TEST_RUN_SUBTEST(CheckVsAtof, (void)0, "1.1e+2");
  TEST_PASS();
}

TEST_FUNC(Sign) {
  TEST_RUN_SUBTEST(CheckVsAtof, (void)0, "-1.1");
  TEST_PASS();
}

TEST_FUNC(Precision) {
  TEST_RUN_SUBTEST(CheckVsAtof, (void)0, "0.999999999999999999999999999999999");
  TEST_PASS();
}

TEST_SUITE_FUNC(StringViewToF32) {
  TEST_RUN(LeadingDot);
  TEST_RUN(Simple);
  TEST_RUN(DecimalPoint);
  TEST_RUN(Exponent);
  TEST_RUN(ExponentWithSign);
  TEST_RUN(Sign);
  TEST_RUN(Precision);
  TEST_PASS();
}

// TEST_SUITE_FUNC(StringViewToF64) {
//   TEST_RUN(LeadingDot64);
//   TEST_RUN(Simple64);
//   TEST_RUN(DecimalPoint64);
//   TEST_RUN(Exponent64);
//   TEST_RUN(ExponentWithSign);
//   TEST_RUN(Sign);
//   TEST_RUN(Precision);
//   TEST_PASS();
// }

int main(void) {
  uint64_t test_count = 0;
  TEST_RUN_SUITE(StringViewToF32, &test_count);
  printf("%" PRIu64 " tests run.\n", test_count);
}
