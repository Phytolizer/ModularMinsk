#include "minsk_test/code_analysis/syntax/lexer_tests.h"

#include <inttypes.h>
#include <minsk/code_analysis/syntax/kind.h>
#include <minsk/code_analysis/syntax/token.h>
#include <minsk/code_analysis/syntax/tree.h>
#include <stddef.h>
#include <stdint.h>
#include <string/string.h>

typedef struct {
  MskSyntaxKind kind;
  const char* text;
} TestToken;

static const TestToken kTokens[] = {
    {kMskSyntaxKindIdentifierToken, "a"},
    {kMskSyntaxKindIdentifierToken, "abc"},
};

TEST_FUNC(LexerLexesToken);

TEST_SUITE_FUNC(LexerTests) {
  TEST_RUN(LexerLexesToken);
}

TEST_FUNC(LexerLexesToken) {
  for (size_t i = 0; i < sizeof(kTokens) / sizeof(TestToken); ++i) {
    const TestToken* tok = &kTokens[i];
    MskSyntaxTokens tokens =
        MskSyntaxTreeParseTokens(StringViewFromC(tok->text));
#define CLEANUP_TOKENS()                       \
  do {                                         \
    for (size_t i = 0; i < tokens.size; ++i) { \
      MskSyntaxTokenFree(&tokens.data[i]);     \
    }                                          \
    VEC_FREE(&tokens);                         \
  } while (false)
    TEST_ASSERT(tokens.size == 1, CLEANUP_TOKENS(),
                "Expected 1 token, got %" PRIu64, tokens.size);
    MskSyntaxToken* actual = &tokens.data[0];
    TEST_ASSERT(actual->kind == tok->kind, CLEANUP_TOKENS(),
                "Expected kind %" STRING_VIEW_FMT ", got %" STRING_VIEW_FMT,
                STRING_VIEW_PRINT(MskSyntaxKindName(tok->kind)),
                STRING_VIEW_PRINT(MskSyntaxKindName(actual->kind)));
    TEST_ASSERT(StringEqualC(actual->text, tok->text), CLEANUP_TOKENS(),
                "Expected text %s, got %" STRING_FMT, tok->text,
                STRING_PRINT(actual->text));
    CLEANUP_TOKENS();
#undef CLEANUP_TOKENS
  }
  TEST_PASS();
}
