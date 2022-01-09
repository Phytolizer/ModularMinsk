#include "nonstd/strspn.h"

size_t nonstd_strspn(const char* s, const char* accept) {
    size_t count = 0;
    for (const char* p = s; *p != '\0'; ++p) {
        const char* a;
        for (a = accept; *a != '\0'; ++a) {
            if (*p == *a) {
                break;
            }
        }
        if (*a == '\0') {
            return count;
        }
        ++count;
    }
    return count;
}
