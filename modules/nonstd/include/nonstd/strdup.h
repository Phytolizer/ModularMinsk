#ifndef NONSTD_STRDUP_H_
#define NONSTD_STRDUP_H_

#include <stddef.h>

char* NonstdStringDuplicate(const char* str);
char* NonstdStringDuplicateN(const char* str, size_t n);
void* NonstdMemoryDuplicate(const void* mem, size_t nbytes);

#endif  // NONSTD_STRDUP_H_
