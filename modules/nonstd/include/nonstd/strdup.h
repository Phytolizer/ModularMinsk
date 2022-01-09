#ifndef NONSTD_STRDUP_H_
#define NONSTD_STRDUP_H_

#include <stddef.h>

char* nonstd_strdup(const char* str);
char* nonstd_strndup(const char* str, size_t n);
void* nonstd_memdup(const void* mem, size_t nbytes);

#endif  // NONSTD_STRDUP_H_
