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

#define STRING_CONVERSION_RESULT(DestType) \
  struct {                                 \
    bool success;                          \
    DestType value;                        \
  }

typedef STRING_CONVERSION_RESULT(int32_t) StringConversionResultI32;
typedef STRING_CONVERSION_RESULT(int64_t) StringConversionResultI64;
typedef STRING_CONVERSION_RESULT(uint32_t) StringConversionResultU32;
typedef STRING_CONVERSION_RESULT(uint64_t) StringConversionResultU64;
typedef STRING_CONVERSION_RESULT(intmax_t) StringConversionResultIMax;
typedef STRING_CONVERSION_RESULT(uintmax_t) StringConversionResultUMax;
typedef STRING_CONVERSION_RESULT(float) StringConversionResultF32;
typedef STRING_CONVERSION_RESULT(double) StringConversionResultF64;

#undef STRING_CONVERSION_RESULT

typedef enum {
  kStringCompareGreater,
  kStringCompareLesser,
  kStringCompareEqual,
} StringCompareResult;

// StringView functions.

/// Returns a StringView over a C string.
StringView StringViewFromC(const char* cstr);
/// Attempts to convert a StringView to an int32_t.
StringConversionResultI32 StringViewToI32(StringView str);
/// Attempts to convert a StringView to an int64_t.
StringConversionResultI64 StringViewToI64(StringView str);
/// Attempts to convert a StringView to a uint32_t.
StringConversionResultU32 StringViewToU32(StringView str);
/// Attempts to convert a StringView to a uint64_t.
StringConversionResultU64 StringViewToU64(StringView str);
/// Attempts to convert a StringView to a float.
StringConversionResultF32 StringViewToF32(StringView str);
/// Attempts to convert a StringView to a double.
StringConversionResultF64 StringViewToF64(StringView str);
/// Attempts to convert a StringView to an intmax_t.
StringConversionResultIMax StringViewToIMax(StringView str);
/// Attempts to convert a StringView to a uintmax_t.
StringConversionResultUMax StringViewToUMax(StringView str);
/// Compares two StringViews.
StringCompareResult StringViewCompare(StringView a, StringView b);
/// Returns a pointer to the found char, or str.end if it wasn't found.
const char* StringViewFindChar(StringView str, char c);
/// Returns a pointer to the found char, or str.end if it wasn't found.
const char* StringViewFindCharReverse(StringView str, char c);
/// Returns the length of the span containing only the characters in chars.
uint64_t StringViewFindSpan(StringView str, StringView chars);
/// Returns the length of the span containing only characters not in chars.
uint64_t StringViewFindSpanReject(StringView str, StringView chars);
/// Searches the string for the first occurrence of any char in breaks.
/// Returns a pointer to the found char, or str.end if it wasn't found.
const char* StringViewFindBreak(StringView str, StringView breaks);
/// Searches the string for the first occurrence of sub.
/// Returns a pointer to the found substring, or an empty span if it wasn't
/// found.
StringView StringViewFind(StringView str, StringView sub);
/// Tokenizes the string using breaks.
StringView StringViewTokenize(StringView str,
                              StringView breaks,
                              StringView* save);
/// Returns a StringView that is a substring of str.
StringView StringViewSubstring(StringView str, uint64_t begin, uint64_t end);
/// Prints a StringView to stdout.
void StringViewPrint(StringView view);
/// Returns the size of a StringView.
uint64_t StringViewSize(StringView view);
/// Returns a StringView of the found substring, or an empty view if no match.
StringView StringViewFindSubstring(StringView haystack, StringView needle);
/// Returns a StringView over 'size' bytes of memory starting at 'ptr'.
StringView StringViewFromPtr(const char* ptr, uint64_t size);
/// Returns true if the StringView is equal to the C string.
bool StringViewEqualC(StringView view, const char* cstr);
/// Returns a C string with the same contents as the StringView, but null
/// terminated. It must be freed by the caller.
char* StringViewToC(StringView view);

// String functions.

/// Copies a C string into a String.
String StringFromC(const char* cstr);
/// Converts a StringView to an owning String.
String StringFromView(StringView view);
/// Returns a copy of the String.
String StringDuplicate(const String s);
/// Takes printf-like format args and creates a String.
String StringFormat(const char* format, ...) STRING_ATTR_PRINTF;
/// Reads a line from FILE into a String.
String StringGetLine(FILE* file);
/// Pushes a single char onto a String.
void StringPush(String* s, char c);
/// Appends a StringView onto a String.
void StringAppendView(String* s, StringView view);
/// Appends a C string onto a String.
void StringAppendC(String* s, const char* cstr);
/// Cleans up a String's allocated memory.
void StringFree(String* s);
/// Converts a String to a non-owning StringView.
StringView StringAsView(const String s);
/// Converts a substring of the String to a StringView.
StringView StringAsSubView(const String s, uint64_t begin, uint64_t end);
/// Returns true if the two Strings are equal.
bool StringEqual(const String a, const String b);
/// Returns true if the String is equal to the StringView.
bool StringEqualView(const String a, StringView b);
/// Returns true if the String is equal to the C string.
bool StringEqualC(const String a, const char* b);

#define STRING_VIEW_PRINT(S) (int)((S).end - (S).begin), (S).begin
#define STRING_VIEW_FMT ".*s"

#define STRING_PRINT(S) (int)(S).size, (S).data
#define STRING_FMT ".*s"

#endif  // STRING_STRING_H_
