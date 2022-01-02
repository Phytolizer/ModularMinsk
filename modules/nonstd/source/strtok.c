#include "nonstd/strtok.h"
#include "nonstd/strpbrk.h"
#include "nonstd/strspn.h"
#include <stddef.h>
#include <string.h>

char* NonstdStrtok(char* str, const char* delim)
{
    static char* olds;
    return NonstdStringTokenizeReentrant(str, delim, &olds);
}

char* NonstdStringTokenizeReentrant(char* str, const char* delim, char** savep)
{
    if (str == NULL)
    {
        str = *savep;
    }

    str += NonstdStringSpan(str, delim);
    if (*str == '\0')
    {
        *savep = str;
        return NULL;
    }

    char* token = str;
    str = NonstdStringPointBreak(token, delim);
    if (str == NULL)
    {
        *savep = strchr(token, '\0');
    }
    else
    {
        *str = '\0';
        *savep = str + 1;
    }
    return token;
}
