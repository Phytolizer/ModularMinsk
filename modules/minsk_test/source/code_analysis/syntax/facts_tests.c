#include "minsk_test/code_analysis/syntax/facts_tests.h"

#include <minsk/code_analysis/syntax/facts.h>
#include <minsk/code_analysis/syntax/kind.h>
#include <minsk/code_analysis/syntax/token.h>
#include <minsk/code_analysis/syntax/tree.h>
#include <stddef.h>
#include <stdint.h>

static TEST_FUNC(GetTextRoundTrips);

TEST_SUITE_FUNC(SyntaxFactsTests) {
  TEST_RUN(GetTextRoundTrips);
}

TEST_FUNC(GetTextRoundTrips) {
  MskSyntaxKinds all_kinds = MskSyntaxKindsGetAll();
  for (uint64_t i = 0; i < VEC_SIZE(&all_kinds); ++i) {
    MskSyntaxKind kind = all_kinds.data[i];
    StringView text = MskSyntaxFactsGetText(kind);
    if (StringViewSize(text) > 0) {
      MskSyntaxTokens tokens = MskSyntaxTreeParseTokens(text);
#define CLEANUP()                 \
  do {                            \
    MskSyntaxTokensFree(&tokens); \
    VEC_FREE(&all_kinds);         \
  } while (false)
      TEST_ASSERT(VEC_SIZE(&tokens) == 1, CLEANUP(),
                  "[%" STRING_VIEW_FMT "] Expected 1 token",
                  STRING_VIEW_PRINT(MskSyntaxKindName(kind)));
      TEST_ASSERT(tokens.data[0].kind == kind, CLEANUP(),
                  "[%" STRING_VIEW_FMT
                  "] Lexer returned kind %" STRING_VIEW_FMT,
                  STRING_VIEW_PRINT(MskSyntaxKindName(kind)),
                  STRING_VIEW_PRINT(MskSyntaxKindName(tokens.data[0].kind)));
      TEST_ASSERT(StringEqualView(tokens.data[0].text, text), CLEANUP(),
                  "[%" STRING_VIEW_FMT "] Lexer returned text '%" STRING_FMT
                  "', not '%" STRING_VIEW_FMT "'",
                  STRING_VIEW_PRINT(MskSyntaxKindName(kind)),
                  STRING_PRINT(tokens.data[0].text), STRING_VIEW_PRINT(text));
      MskSyntaxTokensFree(&tokens);
#undef CLEANUP
    }
  }
  VEC_FREE(&all_kinds);
  TEST_PASS();
}
