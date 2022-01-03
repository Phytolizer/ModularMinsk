#include "string/string.h"

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vec/vec.h>

static uint64_t PowerU64(uint64_t base, uint64_t exponent) {
  uint64_t result = 1;
  for (uint64_t i = 0; i < exponent; ++i) {
    result *= base;
  }
  return result;
}

StringView StringViewFromC(const char* cstr) {
  return (StringView){.begin = cstr, .end = cstr + strlen(cstr)};
}

StringConversionResultI32 StringViewToI32(StringView str) {
  int32_t value = 0;
  for (size_t i = 0; i < str.end - str.begin; ++i) {
    char c = str.begin[i];
    if (c < '0' || c > '9') {
      return (StringConversionResultI32){.success = false};
    }
    // Detect overflow.
    if (value > INT32_MAX / 10 ||
        (value == INT32_MAX / 10 && c - '0' > INT32_MAX % 10)) {
      return (StringConversionResultI32){.success = false};
    }
    value = value * 10 + (c - '0');
  }
  return (StringConversionResultI32){.success = true, .value = value};
}

StringConversionResultI64 StringViewToI64(StringView str) {
  int64_t value = 0;
  for (size_t i = 0; i < SPAN_SIZE(&str); ++i) {
    if (str.begin[i] < '0' || str.begin[i] > '9') {
      return (StringConversionResultI64){.success = false};
    }
    // Detect overflow.
    if (value > INT64_MAX / 10 ||
        (value == INT64_MAX / 10 && str.begin[i] - '0' > INT64_MAX % 10)) {
      return (StringConversionResultI64){.success = false};
    }
    value = value * 10 + (str.begin[i] - '0');
  }
  return (StringConversionResultI64){.success = true, .value = value};
}

StringConversionResultU32 StringViewToU32(StringView str) {
  uint32_t value = 0;
  for (size_t i = 0; i < SPAN_SIZE(&str); ++i) {
    if (str.begin[i] < '0' || str.begin[i] > '9') {
      return (StringConversionResultU32){.success = false};
    }
    // Detect overflow.
    if (value > UINT32_MAX / 10 ||
        (value == UINT32_MAX / 10 && str.begin[i] - '0' > UINT32_MAX % 10)) {
      return (StringConversionResultU32){.success = false};
    }
    value = value * 10 + (str.begin[i] - '0');
  }
  return (StringConversionResultU32){.success = true, .value = value};
}

StringConversionResultU64 StringViewToU64(StringView str) {
  uint64_t value = 0;
  for (size_t i = 0; i < SPAN_SIZE(&str); ++i) {
    if (str.begin[i] < '0' || str.begin[i] > '9') {
      return (StringConversionResultU64){.success = false};
    }
    // Detect overflow.
    if (value > UINT64_MAX / 10 ||
        (value == UINT64_MAX / 10 && str.begin[i] - '0' > UINT64_MAX % 10)) {
      return (StringConversionResultU64){.success = false};
    }
    value = value * 10 + (str.begin[i] - '0');
  }
  return (StringConversionResultU64){.success = true, .value = value};
}

#define GET(str, i) ((i) >= SPAN_SIZE(&str) ? '\0' : str.begin[i])

StringConversionResultF32 StringViewToF32(StringView str) {
  char* text = StringViewToC(str);
  char* end;
  float value = strtof(text, &end);
  bool success = end != text && *end == '\0';
  if (errno != 0) {
    success = false;
    errno = 0;
  }
  free(text);
  return (StringConversionResultF32){.success = success, .value = value};
}

StringConversionResultF64 StringViewToF64(StringView str) {
  char* text = StringViewToC(str);
  char* end;
  double value = strtod(text, &end);
  bool success = end != text && *end == '\0';
  if (errno != 0) {
    success = false;
    errno = 0;
  }
  free(text);
  return (StringConversionResultF64){.success = success, .value = value};
}

#undef GET

StringConversionResultIMax StringViewToIMax(StringView str) {
  intmax_t value = 0;
  for (size_t i = 0; i < SPAN_SIZE(&str); ++i) {
    if (str.begin[i] < '0' || str.begin[i] > '9') {
      return (StringConversionResultIMax){.success = false};
    }
    // Detect overflow.
    if (value > INTMAX_MAX / 10 ||
        (value == INTMAX_MAX / 10 && str.begin[i] - '0' > INTMAX_MAX % 10)) {
      return (StringConversionResultIMax){.success = false};
    }
    value = value * 10 + (str.begin[i] - '0');
  }
  return (StringConversionResultIMax){.success = true, .value = value};
}

StringConversionResultUMax StringViewToUMax(StringView str) {
  uintmax_t value = 0;
  for (size_t i = 0; i < SPAN_SIZE(&str); ++i) {
    if (str.begin[i] < '0' || str.begin[i] > '9') {
      return (StringConversionResultUMax){.success = false};
    }
    // Detect overflow.
    if (value > UINTMAX_MAX / 10 ||
        (value == UINTMAX_MAX / 10 && str.begin[i] - '0' > UINTMAX_MAX % 10)) {
      return (StringConversionResultUMax){.success = false};
    }
    value = value * 10 + (str.begin[i] - '0');
  }
  return (StringConversionResultUMax){.success = true, .value = value};
}

StringCompareResult StringViewCompare(StringView a, StringView b) {
  if (StringViewSize(a) != StringViewSize(b)) {
    return StringViewSize(a) > StringViewSize(b) ? kStringCompareGreater
                                                 : kStringCompareLesser;
  }
  for (size_t i = 0; i < StringViewSize(a); ++i) {
    if (a.begin[i] != b.begin[i]) {
      return a.begin[i] > b.begin[i] ? kStringCompareGreater
                                     : kStringCompareLesser;
    }
  }
  return kStringCompareEqual;
}

StringView StringViewSubstring(StringView str, uint64_t begin, uint64_t end) {
  if (begin >= SPAN_SIZE(&str) || end > SPAN_SIZE(&str) || begin >= end) {
    // An invalid range was specified. Return something sane.
    return (StringView){0};
  }
  return (StringView){.begin = str.begin + begin, .end = str.begin + end};
}

void StringViewPrint(StringView view) {
  fwrite(view.begin, 1, SPAN_SIZE(&view), stdout);
}

uint64_t StringViewSize(StringView view) {
  return SPAN_SIZE(&view);
}

StringView StringViewFindSubstring(StringView haystack, StringView needle) {
  uint64_t haystack_size = StringViewSize(haystack);
  uint64_t needle_size = StringViewSize(needle);
  if (needle_size == 0) {
    // Empty needle.
    return (StringView){0};
  }
  if (haystack_size < needle_size) {
    // Needle is longer than haystack.
    return (StringView){0};
  }
  for (uint64_t i = 0; i <= haystack_size - needle_size; ++i) {
    if (memcmp(haystack.begin + i, needle.begin, needle_size) == 0) {
      return StringViewFromPtr(haystack.begin + i, needle_size);
    }
  }
  return (StringView){0};
}

StringView StringViewFromPtr(const char* ptr, uint64_t size) {
  return (StringView){
      .begin = ptr,
      .end = ptr + size,
  };
}

bool StringViewEqualC(StringView view, const char* cstr) {
  return StringViewSize(view) == strlen(cstr) &&
         memcmp(view.begin, cstr, StringViewSize(view)) == 0;
}

char* StringViewToC(StringView view) {
  char* cstr = malloc(StringViewSize(view) + 1);
  memcpy(cstr, view.begin, StringViewSize(view));
  cstr[StringViewSize(view)] = '\0';
  return cstr;
}

String StringFromC(const char* cstr) {
  String s = {0};
  VEC_APPEND(&s, cstr, strlen(cstr));
  return s;
}

String StringFromView(StringView view) {
  String s = {0};
  StringAppendView(&s, view);
  return s;
}

String StringDuplicate(const String s) {
  String result = {0};
  StringAppendView(&result, StringAsView(s));
  return result;
}

String StringFormat(const char* format, ...) {
  String result = {0};
  va_list args;
  va_start(args, format);
  int size = vsnprintf(NULL, 0, format, args);
  va_end(args);
  if (size < 0) {
    return result;
  }
  VEC_RESIZE(&result, size + 1);
  va_start(args, format);
  vsnprintf(result.data, size + 1, format, args);
  va_end(args);
  result.size = size;
  return result;
}

String StringGetLine(FILE* file) {
  String result = {0};
  char buffer[1024] = {0};
  bool hit_eof = true;
  // Keep reading until we get a newline.
  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    // buffer is a valid C string, since fgets() guarantees it.
    StringAppendC(&result, buffer);
    if (result.data[result.size - 1] == '\n') {
      hit_eof = false;
      break;
    }
  }
  if (hit_eof) {
    // We hit EOF before we found a newline.
    StringAppendC(&result, buffer);
  }
  return result;
}

void StringPushChar(String* s, char c) {
  VEC_PUSH(s, c);
}

void StringAppendView(String* s, StringView view) {
  VEC_APPEND(s, view.begin, view.end - view.begin);
}

void StringAppendC(String* s, const char* cstr) {
  VEC_APPEND(s, cstr, strlen(cstr));
}

void StringFree(String* s) {
  VEC_FREE(s);
}

StringView StringAsView(const String s) {
  return (StringView){
      .begin = s.data,
      .end = s.data + s.size,
  };
}

StringView StringAsSubView(const String s, uint64_t begin, uint64_t end) {
  if (begin >= s.size || end > s.size || begin >= end) {
    // An invalid range was specified. Return something sane.
    return (StringView){0};
  }
  return (StringView){
      .begin = s.data + begin,
      .end = s.data + end,
  };
}

bool StringEqual(const String a, const String b) {
  return a.size == b.size && memcmp(a.data, b.data, a.size) == 0;
}

bool StringEqualView(const String a, StringView b) {
  return a.size == SPAN_SIZE(&b) && memcmp(a.data, b.begin, a.size) == 0;
}

bool StringEqualC(const String a, const char* b) {
  return a.size == strlen(b) && memcmp(a.data, b, a.size) == 0;
}
