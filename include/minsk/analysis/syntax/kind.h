#pragma once

#define SYNTAX_KINDS_X                                                                             \
    X(BAD_TOKEN)                                                                                   \
    X(END_OF_FILE_TOKEN)                                                                           \
    X(NUMBER_TOKEN)                                                                                \
    X(WHITESPACE_TOKEN)                                                                            \
    X(PLUS_TOKEN)                                                                                  \
    X(MINUS_TOKEN)                                                                                 \
    X(STAR_TOKEN)                                                                                  \
    X(SLASH_TOKEN)                                                                                 \
    X(OPEN_PARENTHESIS_TOKEN)                                                                      \
    X(CLOSE_PARENTHESIS_TOKEN)

typedef enum {
#define X(x) SYNTAX_KIND_##x,
    SYNTAX_KINDS_X
#undef X
        SYNTAX_KINDS_COUNT
} syntax_kind_t;

const char* syntax_kind_to_string(syntax_kind_t kind);
