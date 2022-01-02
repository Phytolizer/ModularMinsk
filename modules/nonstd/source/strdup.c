#include "nonstd/strdup.h"
#include <stdlib.h>
#include <string.h>

char* NonstdStringDuplicate(const char* str)
{
    size_t len = strlen(str);
    char* result = malloc(len + 1);
    memcpy(result, str, len);
    result[len] = '\0';
    return result;
}

char* NonstdStringDuplicateN(const char* str, size_t n)
{
    char* result = malloc(n + 1);
    memcpy(result, str, n);
    result[n] = '\0';
    return result;
}

void* NonstdMemoryDuplicate(const void* mem, size_t nbytes)
{
    void* result = malloc(nbytes);
    memcpy(result, mem, nbytes);
    return result;
}
