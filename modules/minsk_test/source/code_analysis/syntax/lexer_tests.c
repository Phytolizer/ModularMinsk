#include "minsk_test/code_analysis/syntax/lexer_tests.h"

#include <inttypes.h>
#include <minsk/code_analysis/syntax/kind.h>
#include <minsk/code_analysis/syntax/token.h>
#include <minsk/code_analysis/syntax/tree.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string/string.h>

static TEST_FUNC(LexerLexesToken);
static TEST_FUNC(LexerLexesTokenPairs);

TEST_SUITE_FUNC(LexerTests) {
  TEST_RUN(LexerLexesToken);
  TEST_RUN(LexerLexesTokenPairs);
}

typedef struct {
  MskSyntaxKind kind;
  const char* text;
} TestToken;

static const TestToken kTokens[] = {
    {kMskSyntaxKindIdentifierToken, "a"},
    {kMskSyntaxKindIdentifierToken, "abc"},
    // {kMskSyntaxKindWhitespaceToken, " "},
    // {kMskSyntaxKindWhitespaceToken, "  "},
    // {kMskSyntaxKindWhitespaceToken, "\r"},
    // {kMskSyntaxKindWhitespaceToken, "\n"},
    // {kMskSyntaxKindWhitespaceToken, "\r\n"},

    {kMskSyntaxKindNumberToken, "1"},
    {kMskSyntaxKindNumberToken, "123"},
    {kMskSyntaxKindPlusToken, "+"},
    {kMskSyntaxKindMinusToken, "-"},
    {kMskSyntaxKindStarToken, "*"},
    {kMskSyntaxKindSlashToken, "/"},
    {kMskSyntaxKindOpenParenthesisToken, "("},
    {kMskSyntaxKindCloseParenthesisToken, ")"},
    {kMskSyntaxKindBangToken, "!"},
    {kMskSyntaxKindBangEqualsToken, "!="},
    {kMskSyntaxKindEqualsToken, "="},
    {kMskSyntaxKindEqualsEqualsToken, "=="},
    {kMskSyntaxKindAmpersandAmpersandToken, "&&"},
    {kMskSyntaxKindPipePipeToken, "||"},
    {kMskSyntaxKindTrueKeyword, "true"},
    {kMskSyntaxKindFalseKeyword, "false"},
};

typedef struct {
  TestToken t1;
  TestToken t2;
} TestTokenPair;

typedef VEC_TYPE(TestTokenPair) TestTokenPairs;

static bool RequiresSeparator(MskSyntaxKind t1_kind, MskSyntaxKind t2_kind) {
  bool t1_is_keyword = StringViewEndsWith(MskSyntaxKindName(t1_kind),
                                          StringViewFromC("Keyword"));
  bool t2_is_keyword = StringViewEndsWith(MskSyntaxKindName(t2_kind),
                                          StringViewFromC("Keyword"));
  if ((t1_is_keyword || t1_kind == kMskSyntaxKindIdentifierToken) &&
      (t2_is_keyword || t2_kind == kMskSyntaxKindIdentifierToken)) {
    return true;
  }
  if ((t1_kind == kMskSyntaxKindIdentifierToken || t1_is_keyword ||
       t1_kind == kMskSyntaxKindNumberToken) &&
      t2_kind == kMskSyntaxKindNumberToken) {
    return true;
  }
  if ((t1_kind == kMskSyntaxKindBangToken ||
       t1_kind == kMskSyntaxKindEqualsToken) &&
      (t2_kind == kMskSyntaxKindEqualsToken ||
       t2_kind == kMskSyntaxKindEqualsEqualsToken)) {
    return true;
  }
  return false;
}

static TestTokenPairs GetTokenPairs(void) {
  TestTokenPairs pairs = {0};
  for (size_t i = 0; i < sizeof(kTokens) / sizeof(TestToken); i++) {
    for (size_t j = i + 1; j < sizeof(kTokens) / sizeof(TestToken); j++) {
      if (!RequiresSeparator(kTokens[i].kind, kTokens[j].kind)) {
        TestTokenPair pair = {kTokens[i], kTokens[j]};
        VEC_PUSH(&pairs, pair);
      }
    }
  }
  return pairs;
}

#define CLEANUP_TOKENS()                       \
  do {                                         \
    for (size_t i = 0; i < tokens.size; ++i) { \
      MskSyntaxTokenFree(&tokens.data[i]);     \
    }                                          \
    VEC_FREE(&tokens);                         \
  } while (false)

TEST_FUNC(LexerLexesToken) {
  for (size_t i = 0; i < sizeof(kTokens) / sizeof(TestToken); ++i) {
    const TestToken* tok = &kTokens[i];
    MskSyntaxTokens tokens =
        MskSyntaxTreeParseTokens(StringViewFromC(tok->text));
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
  }
  TEST_PASS();
}

TEST_FUNC(LexerLexesTokenPairs) {
  TestTokenPairs pairs = GetTokenPairs();
  for (uint64_t i = 0; i < pairs.size; ++i) {
    const TestTokenPair* pair = &pairs.data[i];
    String text = {0};
    StringAppendC(&text, pair->t1.text);
    StringAppendC(&text, pair->t2.text);
    MskSyntaxTokens tokens = MskSyntaxTreeParseTokens(StringAsView(text));
    StringFree(&text);
    TEST_ASSERT(tokens.size == 2, CLEANUP_TOKENS(),
                "['%s' '%s'] Expected 2 tokens, got %" PRIu64, pair->t1.text,
                pair->t2.text, tokens.size);
    MskSyntaxToken* actual1 = &tokens.data[0];
    MskSyntaxToken* actual2 = &tokens.data[1];
    TEST_ASSERT(actual1->kind == pair->t1.kind, CLEANUP_TOKENS(),
                "Expected kind %" STRING_VIEW_FMT ", got %" STRING_VIEW_FMT,
                STRING_VIEW_PRINT(MskSyntaxKindName(pair->t1.kind)),
                STRING_VIEW_PRINT(MskSyntaxKindName(actual1->kind)));
    TEST_ASSERT(StringEqualC(actual1->text, pair->t1.text), CLEANUP_TOKENS(),
                "Expected text '%s', got '%" STRING_FMT "'", pair->t1.text,
                STRING_PRINT(actual1->text));
    CLEANUP_TOKENS();
  }
  TEST_PASS();
}
