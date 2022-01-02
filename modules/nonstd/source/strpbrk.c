#include "nonstd/strpbrk.h"
#include <stddef.h>

char* NonstdStringPointBreak(const char* s, const char* accept)
{
    while (*s != '\0')
    {
        const char* a = accept;
        while (*a != '\0')
        {
            if (*a++ == *s)
            {
                return (char*)s;
            }
            ++s;
        }
    }
    return NULL;
}
