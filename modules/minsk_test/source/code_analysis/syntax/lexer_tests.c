#include "minsk_test/code_analysis/syntax/lexer_tests.h"

#include <inttypes.h>
#include <minsk/code_analysis/syntax/kind.h>
#include <minsk/code_analysis/syntax/token.h>
#include <minsk/code_analysis/syntax/tree.h>
#include <phyto/string/string.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_DECL(TestTokenPairs, TestTokenPair);
PHYTO_COLLECTIONS_DYNAMIC_ARRAY_IMPL(TestTokenPairs, TestTokenPair);

static const TestTokenPairs_callbacks_t kTestTokenPairsCallbacks = {0};

static bool RequiresSeparator(MskSyntaxKind t1_kind, MskSyntaxKind t2_kind) {
  bool t1_is_keyword = phyto_string_ends_with(
      MskSyntaxKindName(t1_kind), phyto_string_span_from_c("Keyword"));
  bool t2_is_keyword = phyto_string_ends_with(
      MskSyntaxKindName(t2_kind), phyto_string_span_from_c("Keyword"));
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

static TestTokenPairs_t GetTokenPairs(void) {
  TestTokenPairs_t pairs = TestTokenPairs_init(&kTestTokenPairsCallbacks);
  for (size_t i = 0; i < sizeof(kTokens) / sizeof(TestToken); i++) {
    for (size_t j = i + 1; j < sizeof(kTokens) / sizeof(TestToken); j++) {
      if (!RequiresSeparator(kTokens[i].kind, kTokens[j].kind)) {
        TestTokenPair pair = {kTokens[i], kTokens[j]};
        TestTokenPairs_append(&pairs, pair);
      }
    }
  }
  return pairs;
}

typedef struct {
  TestTokenPair pair;
  TestToken separator;
} TestTokenPairWithSeparator;

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_DECL(TestTokenPairsWithSeparator,
                                     TestTokenPairWithSeparator);
PHYTO_COLLECTIONS_DYNAMIC_ARRAY_IMPL(TestTokenPairsWithSeparator,
                                     TestTokenPairWithSeparator);

static const TestTokenPairsWithSeparator_callbacks_t
    kTestTokenPairsWithSeparatorCallbacks = {0};

static TestTokenPairsWithSeparator_t GetTokenPairsWithSeparator(void) {
  TestTokenPairsWithSeparator_t pairs =
      TestTokenPairsWithSeparator_init(&kTestTokenPairsWithSeparatorCallbacks);
  for (size_t i = 0; i < sizeof(kTokens) / sizeof(TestToken); i++) {
    for (size_t j = i + 1; j < sizeof(kTokens) / sizeof(TestToken); j++) {
      if (RequiresSeparator(kTokens[i].kind, kTokens[j].kind)) {
        for (size_t k = 0; k < sizeof(kSeparators) / sizeof(TestToken); k++) {
          TestTokenPairWithSeparator pair = {{kTokens[i], kTokens[j]},
                                             kSeparators[k]};
          TestTokenPairsWithSeparator_append(&pairs, pair);
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
    MskSyntaxTokens_t tokens =
        MskSyntaxTreeParseTokens(phyto_string_span_from_c(tok->text));
    TEST_ASSERT(tokens.size == 1, MskSyntaxTokens_free(&tokens),
                "Expected 1 token, got %" PRIu64, tokens.size);
    MskSyntaxToken* actual = &tokens.data[0];
    TEST_ASSERT(actual->kind == tok->kind, MskSyntaxTokens_free(&tokens),
                "Expected kind %" PHYTO_STRING_FORMAT
                ", got %" PHYTO_STRING_FORMAT,
                PHYTO_STRING_VIEW_PRINTF_ARGS(MskSyntaxKindName(tok->kind)),
                PHYTO_STRING_VIEW_PRINTF_ARGS(MskSyntaxKindName(actual->kind)));
    TEST_ASSERT(phyto_string_span_equal(phyto_string_as_span(actual->text),
                                        phyto_string_span_from_c(tok->text)),
                MskSyntaxTokens_free(&tokens),
                "Expected text %s, got %" PHYTO_STRING_FORMAT, tok->text,
                PHYTO_STRING_PRINTF_ARGS(actual->text));
    MskSyntaxTokens_free(&tokens);
  }
  TEST_PASS();
}

TEST_FUNC(LexerLexesTokenPairs) {
  TestTokenPairs_t pairs = GetTokenPairs();
  for (uint64_t i = 0; i < pairs.size; ++i) {
    const TestTokenPair* pair = &pairs.data[i];
    phyto_string_t text = phyto_string_new();
    phyto_string_append_c(&text, pair->t1.text);
    phyto_string_append_c(&text, pair->t2.text);
    MskSyntaxTokens_t tokens =
        MskSyntaxTreeParseTokens(phyto_string_as_span(text));
    phyto_string_free(&text);
    TEST_ASSERT(tokens.size == 2, MskSyntaxTokens_free(&tokens),
                "['%s' '%s'] Expected 2 tokens, got %" PRIu64, pair->t1.text,
                pair->t2.text, tokens.size);
    MskSyntaxToken* actual1 = &tokens.data[0];
    MskSyntaxToken* actual2 = &tokens.data[1];
    TEST_ASSERT(
        actual1->kind == pair->t1.kind, MskSyntaxTokens_free(&tokens),
        "Expected kind %" PHYTO_STRING_FORMAT ", got %" PHYTO_STRING_FORMAT,
        PHYTO_STRING_VIEW_PRINTF_ARGS(MskSyntaxKindName(pair->t1.kind)),
        PHYTO_STRING_VIEW_PRINTF_ARGS(MskSyntaxKindName(actual1->kind)));
    TEST_ASSERT(
        phyto_string_span_equal(phyto_string_as_span(actual1->text),
                                phyto_string_span_from_c(pair->t1.text)),
        MskSyntaxTokens_free(&tokens),
        "Expected text '%s', got '%" PHYTO_STRING_FORMAT "'", pair->t1.text,
        PHYTO_STRING_PRINTF_ARGS(actual1->text));
    MskSyntaxTokens_free(&tokens);
  }
  TestTokenPairs_free(&pairs);
  TEST_PASS();
}

TEST_FUNC(LexerLexesTokenPairsWithSeparator) {
  TestTokenPairsWithSeparator_t pairs = GetTokenPairsWithSeparator();
  for (uint64_t i = 0; i < pairs.size; ++i) {
    const TestTokenPairWithSeparator* pair = &pairs.data[i];
    phyto_string_t text = phyto_string_new();
    phyto_string_append_c(&text, pair->pair.t1.text);
    phyto_string_append_c(&text, pair->separator.text);
    phyto_string_append_c(&text, pair->pair.t2.text);
    MskSyntaxTokens_t tokens =
        MskSyntaxTreeParseTokens(phyto_string_as_span(text));
    phyto_string_free(&text);

#define CLEANUP()                             \
  do {                                        \
    MskSyntaxTokens_free(&tokens);            \
    TestTokenPairsWithSeparator_free(&pairs); \
  } while (false)

    TEST_ASSERT(tokens.size == 3, CLEANUP(),
                "['%s' '%s' '%s'] Expected 3 tokens, got %" PRIu64,
                pair->pair.t1.text, pair->separator.text, pair->pair.t2.text,
                tokens.size);
    MskSyntaxToken* actual1 = &tokens.data[0];
    MskSyntaxToken* actual2 = &tokens.data[1];
    MskSyntaxToken* actual3 = &tokens.data[2];
    TEST_ASSERT(
        actual1->kind == pair->pair.t1.kind, CLEANUP(),
        "Expected kind %" PHYTO_STRING_FORMAT ", got %" PHYTO_STRING_FORMAT,
        PHYTO_STRING_VIEW_PRINTF_ARGS(MskSyntaxKindName(pair->pair.t1.kind)),
        PHYTO_STRING_VIEW_PRINTF_ARGS(MskSyntaxKindName(actual1->kind)));
    TEST_ASSERT(
        phyto_string_span_equal(phyto_string_as_span(actual1->text),
                                phyto_string_span_from_c(pair->pair.t1.text)),
        CLEANUP(), "Expected text '%s', got '%" PHYTO_STRING_FORMAT "'",
        pair->pair.t1.text, PHYTO_STRING_PRINTF_ARGS(actual1->text));
    TEST_ASSERT(
        actual2->kind == pair->separator.kind, CLEANUP(),
        "Expected kind %" PHYTO_STRING_FORMAT ", got %" PHYTO_STRING_FORMAT,
        PHYTO_STRING_VIEW_PRINTF_ARGS(MskSyntaxKindName(pair->separator.kind)),
        PHYTO_STRING_VIEW_PRINTF_ARGS(MskSyntaxKindName(actual2->kind)));
    TEST_ASSERT(
        phyto_string_span_equal(phyto_string_as_span(actual2->text),
                                phyto_string_span_from_c(pair->separator.text)),
        CLEANUP(), "Expected text '%s', got '%" PHYTO_STRING_FORMAT "'",
        pair->separator.text, PHYTO_STRING_PRINTF_ARGS(actual2->text));
    TEST_ASSERT(
        actual3->kind == pair->pair.t2.kind, CLEANUP(),
        "Expected kind %" PHYTO_STRING_FORMAT ", got %" PHYTO_STRING_FORMAT,
        PHYTO_STRING_VIEW_PRINTF_ARGS(MskSyntaxKindName(pair->pair.t2.kind)),
        PHYTO_STRING_VIEW_PRINTF_ARGS(MskSyntaxKindName(actual3->kind)));
    TEST_ASSERT(
        phyto_string_span_equal(phyto_string_as_span(actual3->text),
                                phyto_string_span_from_c(pair->pair.t2.text)),
        CLEANUP(), "Expected text '%s', got '%" PHYTO_STRING_FORMAT "'",
        pair->pair.t2.text, PHYTO_STRING_PRINTF_ARGS(actual3->text));
    MskSyntaxTokens_free(&tokens);
#undef CLEANUP
  }
  TestTokenPairsWithSeparator_free(&pairs);
  TEST_PASS();
}
