#include "nonstd/strspn.h"

size_t NonstdStringSpan(const char* s, const char* accept)
{
    size_t count = 0;
    for (const char* p = s; *p != '\0'; ++p)
    {
        const char* a;
        for (a = accept; *a != '\0'; ++a)
        {
            if (*p == *a)
            {
                break;
            }
        }
        if (*a == '\0')
        {
            return count;
        }
        else
        {
            ++count;
        }
    }
    return count;
}
