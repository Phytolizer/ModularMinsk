#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>
#include <string/string.h>
#include <test/test.h>

TEST_FUNC(LeadingDot) {
  StringView text = StringViewFromC(".1");
  StringConversionResultF32 result = StringViewToF32(text);
  TEST_ASSERT(result.success, (void)0, "Expected success.");
  float expected_result = atof(".1");
  TEST_ASSERT(result.value == expected_result, (void)0, "Expected %f, got %f.",
              expected_result, result.value);
  TEST_PASS();
}

TEST_FUNC(Simple) {
  StringView text = StringViewFromC("1");
  StringConversionResultF32 result = StringViewToF32(text);
  TEST_ASSERT(result.success, (void)0, "Expected success.");
  float expected_result = atof("1");
  TEST_ASSERT(result.value == expected_result, (void)0, "Expected %f, got %f.",
              expected_result, result.value);
  TEST_PASS();
}

TEST_FUNC(DecimalPoint) {
  StringView text = StringViewFromC("1.1");
  StringConversionResultF32 result = StringViewToF32(text);
  TEST_ASSERT(result.success, (void)0, "Expected success.");
  float expected_result = atof("1.1");
  TEST_ASSERT(result.value == expected_result, (void)0, "Expected %f, got %f.",
              expected_result, result.value);
  TEST_PASS();
}

TEST_FUNC(Exponent) {
  StringView text = StringViewFromC("1.1e2");
  StringConversionResultF32 result = StringViewToF32(text);
  TEST_ASSERT(result.success, (void)0, "Expected success.");
  float expected_result = atof("1.1e2");
  TEST_ASSERT(result.value == expected_result, (void)0, "Expected %f, got %f.",
              expected_result, result.value);
  TEST_PASS();
}

TEST_FUNC(ExponentWithSign) {
  StringView text = StringViewFromC("1.1e+2");
  StringConversionResultF32 result = StringViewToF32(text);
  TEST_ASSERT(result.success, (void)0, "Expected success.");
  float expected_result = atof("1.1e+2");
  TEST_ASSERT(result.value == expected_result, (void)0, "Expected %f, got %f.",
              expected_result, result.value);
  TEST_PASS();
}

TEST_FUNC(Sign) {
  StringView text = StringViewFromC("-1.1");
  StringConversionResultF32 result = StringViewToF32(text);
  TEST_ASSERT(result.success, (void)0, "Expected success.");
  float expected_result = atof("-1.1");
  TEST_ASSERT(result.value == expected_result, (void)0, "Expected %f, got %f.",
              expected_result, result.value);
  TEST_PASS();
}

TEST_FUNC(Precision) {
  StringView text = StringViewFromC("0.9999999999999999999999999999999999");
  StringConversionResultF32 result = StringViewToF32(text);
  TEST_ASSERT(result.success, (void)0, "Expected success.");
  float expected_result = atof("0.9999999999999999999999999999999999");
  TEST_ASSERT(result.value == expected_result, (void)0, "Expected %f, got %f.",
              expected_result, result.value);
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
