#ifndef NONSTD_CTYPE_H_
#define NONSTD_CTYPE_H_

#include <stdbool.h>

char nonstd_tolower(char c);
char nonstd_toupper(char c);
bool nonstd_isalpha(char c);
bool nonstd_isdigit(char c);
bool nonstd_isalnum(char c);
bool nonstd_islower(char c);
bool nonstd_isupper(char c);
bool nonstd_isprint(char c);
bool nonstd_isspace(char c);

#endif  // NONSTD_CTYPE_H_
