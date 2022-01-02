#ifndef STRING_STRING_H_
#define STRING_STRING_H_

#include <span/span.h>
#include <stdbool.h>
#include <vec/vec.h>

typedef VEC_TYPE(char) String;
typedef SPAN_TYPE(char) StringView;

#ifdef __GNUC__
#define STRING_ATTR_PRINTF __attribute__((format(printf, 1, 2)))
#else
#define STRING_ATTR_PRINTF
#endif

StringView StringViewFromC(const char* cstr);
String StringFromC(const char* cstr);
String StringFromSpan(StringView span);
String StringDuplicate(const String s);
String StringFormat(const char* format, ...) STRING_ATTR_PRINTF;
bool StringEqual(const String a, const String b);
bool StringEqualView(const String a, StringView b);

#define STRING_PRINT(S) (int)(S).size, (S).data
#define STRING_FMT ".*s"

#endif  // STRING_STRING_H_
