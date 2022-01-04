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
static TEST_FUNC(LexerLexesTokenPairsWithSeparator);

TEST_SUITE_FUNC(LexerTests) {
  TEST_RUN(LexerLexesToken);
  TEST_RUN(LexerLexesTokenPairs);
  TEST_RUN(LexerLexesTokenPairsWithSeparator);
}

typedef struct {
  MskSyntaxKind kind;
  const char* text;
} TestToken;

static const TestToken kTokens[] = {
    {kMskSyntaxKindIdentifierToken, "a"},
    {kMskSyntaxKindIdentifierToken, "abc"},

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

static const TestToken kSeparators[] = {
    {kMskSyntaxKindWhitespaceToken, " "},
    {kMskSyntaxKindWhitespaceToken, "  "},
    {kMskSyntaxKindWhitespaceToken, "\r"},
    {kMskSyntaxKindWhitespaceToken, "\n"},
    {kMskSyntaxKindWhitespaceToken, "\r\n"},
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

typedef struct {
  TestTokenPair pair;
  TestToken separator;
} TestTokenPairWithSeparator;

typedef VEC_TYPE(TestTokenPairWithSeparator) TestTokenPairsWithSeparator;

static TestTokenPairsWithSeparator GetTokenPairsWithSeparator(void) {
  TestTokenPairsWithSeparator pairs = {0};
  for (size_t i = 0; i < sizeof(kTokens) / sizeof(TestToken); i++) {
    for (size_t j = i + 1; j < sizeof(kTokens) / sizeof(TestToken); j++) {
      if (RequiresSeparator(kTokens[i].kind, kTokens[j].kind)) {
        for (size_t k = 0; k < sizeof(kSeparators) / sizeof(TestToken); k++) {
          TestTokenPairWithSeparator pair = {{kTokens[i], kTokens[j]},
                                             kSeparators[k]};
          VEC_PUSH(&pairs, pair);
        }
      }
    }
  }
  return pairs;
}

TEST_FUNC(LexerLexesToken) {
  size_t num_tokens = sizeof(kTokens) / sizeof(TestToken);
  for (size_t i = 0; i < num_tokens + sizeof(kSeparators) / sizeof(TestToken);
       ++i) {
    const TestToken* tok =
        i < num_tokens ? &kTokens[i] : &kSeparators[i - num_tokens];
    MskSyntaxTokens tokens =
        MskSyntaxTreeParseTokens(StringViewFromC(tok->text));
    TEST_ASSERT(tokens.size == 1, MskSyntaxTokensFree(&tokens),
                "Expected 1 token, got %" PRIu64, tokens.size);
    MskSyntaxToken* actual = &tokens.data[0];
    TEST_ASSERT(actual->kind == tok->kind, MskSyntaxTokensFree(&tokens),
                "Expected kind %" STRING_VIEW_FMT ", got %" STRING_VIEW_FMT,
                STRING_VIEW_PRINT(MskSyntaxKindName(tok->kind)),
                STRING_VIEW_PRINT(MskSyntaxKindName(actual->kind)));
    TEST_ASSERT(StringEqualC(actual->text, tok->text),
                MskSyntaxTokensFree(&tokens),
                "Expected text %s, got %" STRING_FMT, tok->text,
                STRING_PRINT(actual->text));
    MskSyntaxTokensFree(&tokens);
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
    TEST_ASSERT(tokens.size == 2, MskSyntaxTokensFree(&tokens),
                "['%s' '%s'] Expected 2 tokens, got %" PRIu64, pair->t1.text,
                pair->t2.text, tokens.size);
    MskSyntaxToken* actual1 = &tokens.data[0];
    MskSyntaxToken* actual2 = &tokens.data[1];
    TEST_ASSERT(actual1->kind == pair->t1.kind, MskSyntaxTokensFree(&tokens),
                "Expected kind %" STRING_VIEW_FMT ", got %" STRING_VIEW_FMT,
                STRING_VIEW_PRINT(MskSyntaxKindName(pair->t1.kind)),
                STRING_VIEW_PRINT(MskSyntaxKindName(actual1->kind)));
    TEST_ASSERT(StringEqualC(actual1->text, pair->t1.text),
                MskSyntaxTokensFree(&tokens),
                "Expected text '%s', got '%" STRING_FMT "'", pair->t1.text,
                STRING_PRINT(actual1->text));
    MskSyntaxTokensFree(&tokens);
  }
  VEC_FREE(&pairs);
  TEST_PASS();
}

TEST_FUNC(LexerLexesTokenPairsWithSeparator) {
  TestTokenPairsWithSeparator pairs = GetTokenPairsWithSeparator();
  for (uint64_t i = 0; i < pairs.size; ++i) {
    const TestTokenPairWithSeparator* pair = &pairs.data[i];
    String text = {0};
    StringAppendC(&text, pair->pair.t1.text);
    StringAppendC(&text, pair->separator.text);
    StringAppendC(&text, pair->pair.t2.text);
    MskSyntaxTokens tokens = MskSyntaxTreeParseTokens(StringAsView(text));
    StringFree(&text);
    TEST_ASSERT(tokens.size == 3, MskSyntaxTokensFree(&tokens),
                "['%s' '%s' '%s'] Expected 3 tokens, got %" PRIu64,
                pair->pair.t1.text, pair->separator.text, pair->pair.t2.text,
                tokens.size);
    MskSyntaxToken* actual1 = &tokens.data[0];
    MskSyntaxToken* actual2 = &tokens.data[1];
    MskSyntaxToken* actual3 = &tokens.data[2];
    TEST_ASSERT(actual1->kind == pair->pair.t1.kind,
                MskSyntaxTokensFree(&tokens),
                "Expected kind %" STRING_VIEW_FMT ", got %" STRING_VIEW_FMT,
                STRING_VIEW_PRINT(MskSyntaxKindName(pair->pair.t1.kind)),
                STRING_VIEW_PRINT(MskSyntaxKindName(actual1->kind)));
    TEST_ASSERT(StringEqualC(actual1->text, pair->pair.t1.text),
                MskSyntaxTokensFree(&tokens),
                "Expected text '%s', got '%" STRING_FMT "'", pair->pair.t1.text,
                STRING_PRINT(actual1->text));
    TEST_ASSERT(actual2->kind == pair->separator.kind,
                MskSyntaxTokensFree(&tokens),
                "Expected kind %" STRING_VIEW_FMT ", got %" STRING_VIEW_FMT,
                STRING_VIEW_PRINT(MskSyntaxKindName(pair->separator.kind)),
                STRING_VIEW_PRINT(MskSyntaxKindName(actual2->kind)));
    TEST_ASSERT(StringEqualC(actual2->text, pair->separator.text),
                MskSyntaxTokensFree(&tokens),
                "Expected text '%s', got '%" STRING_FMT "'",
                pair->separator.text, STRING_PRINT(actual2->text));
    TEST_ASSERT(actual3->kind == pair->pair.t2.kind,
                MskSyntaxTokensFree(&tokens),
                "Expected kind %" STRING_VIEW_FMT ", got %" STRING_VIEW_FMT,
                STRING_VIEW_PRINT(MskSyntaxKindName(pair->pair.t2.kind)),
                STRING_VIEW_PRINT(MskSyntaxKindName(actual3->kind)));
    TEST_ASSERT(StringEqualC(actual3->text, pair->pair.t2.text),
                MskSyntaxTokensFree(&tokens),
                "Expected text '%s', got '%" STRING_FMT "'", pair->pair.t2.text,
                STRING_PRINT(actual3->text));
    MskSyntaxTokensFree(&tokens);
  }
  VEC_FREE(&pairs);
  TEST_PASS();
}
