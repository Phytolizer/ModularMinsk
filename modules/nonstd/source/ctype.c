#include "nonstd/ctype.h"

char nonstd_tolower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return (char)(c + ('a' - 'A'));
    }
    return c;
}

char nonstd_toupper(char c) {
    if (c >= 'a' && c <= 'z') {
        return (char)(c + ('A' - 'a'));
    }
    return c;
}

bool nonstd_isalpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool nonstd_isdigit(char c) {
    return c >= '0' && c <= '9';
}

bool nonstd_isalnum(char c) {
    return nonstd_isalpha(c) || nonstd_isdigit(c);
}

bool nonstd_islower(char c) {
    return c >= 'a' && c <= 'z';
}

bool nonstd_isupper(char c) {
    return c >= 'A' && c <= 'Z';
}

bool nonstd_isprint(char c) {
    return c >= ' ' && c <= '~';
}

bool nonstd_isspace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}
