#ifndef STRING_STRING_H_
#define STRING_STRING_H_

#include <span/span.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <vec/vec.h>

typedef VEC_TYPE(char) String;
typedef SPAN_TYPE(char) StringView;

#ifdef __GNUC__
#define STRING_ATTR_PRINTF __attribute__((format(printf, 1, 2)))
#else
#define STRING_ATTR_PRINTF
#endif

typedef struct {
  bool success;
  int64_t value;
} StringConversionResultI64;

StringView StringViewFromC(const char* cstr);
StringConversionResultI64 StringViewToI64(StringView str);
StringView StringViewSubstring(StringView str, uint64_t begin, uint64_t end);
String StringFromC(const char* cstr);
String StringFromView(StringView span);
String StringDuplicate(const String s);
String StringFormat(const char* format, ...) STRING_ATTR_PRINTF;
String StringGetLine(FILE* file);
void StringPushChar(String* s, char c);
void StringAppendView(String* s, StringView view);
void StringAppendC(String* s, const char* cstr);
void StringFree(String* s);
StringView StringAsView(const String s);
StringView StringAsSubView(const String s, uint64_t begin, uint64_t end);
bool StringEqual(const String a, const String b);
bool StringEqualView(const String a, StringView b);

#define STRING_VIEW_PRINT(S) (int)((S).end - (S).begin), (S).begin
#define STRING_VIEW_FMT ".*s"

#define STRING_PRINT(S) (int)(S).size, (S).data
#define STRING_FMT ".*s"

#endif  // STRING_STRING_H_
