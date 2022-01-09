#include "nonstd/strdup.h"

#include <stdlib.h>
#include <string.h>

char* nonstd_strdup(const char* str) {
    size_t len = strlen(str);
    char* result = malloc(len + 1);
    memcpy(result, str, len);
    result[len] = '\0';
    return result;
}

char* nonstd_strndup(const char* str, size_t n) {
    char* result = malloc(n + 1);
    memcpy(result, str, n);
    result[n] = '\0';
    return result;
}

void* nonstd_memdup(const void* mem, size_t nbytes) {
    void* result = malloc(nbytes);
    memcpy(result, mem, nbytes);
    return result;
}
