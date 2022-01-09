#include "minsk_test/code_analysis/syntax/facts_tests.h"

#include <minsk/code_analysis/syntax/facts.h>
#include <minsk/code_analysis/syntax/kind.h>
#include <minsk/code_analysis/syntax/token.h>
#include <minsk/code_analysis/syntax/tree.h>
#include <phyto/string/string.h>
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
    phyto_string_span_t text = MskSyntaxFactsGetText(kind);
    if (text.size > 0) {
      MskSyntaxTokens tokens = MskSyntaxTreeParseTokens(text);
#define CLEANUP()                 \
  do {                            \
    MskSyntaxTokensFree(&tokens); \
    VEC_FREE(&all_kinds);         \
  } while (false)
      TEST_ASSERT(VEC_SIZE(&tokens) == 1, CLEANUP(),
                  "[%" PHYTO_STRING_FORMAT "] Expected 1 token",
                  PHYTO_STRING_VIEW_PRINTF_ARGS(MskSyntaxKindName(kind)));
      TEST_ASSERT(tokens.data[0].kind == kind, CLEANUP(),
                  "[%" PHYTO_STRING_FORMAT
                  "] Lexer returned kind %" PHYTO_STRING_FORMAT,
                  PHYTO_STRING_VIEW_PRINTF_ARGS(MskSyntaxKindName(kind)),
                  PHYTO_STRING_VIEW_PRINTF_ARGS(
                      MskSyntaxKindName(tokens.data[0].kind)));
      TEST_ASSERT(phyto_string_span_equal(
                      phyto_string_as_span(tokens.data[0].text), text),
                  CLEANUP(),
                  "[%" PHYTO_STRING_FORMAT
                  "] Lexer returned text '%" PHYTO_STRING_FORMAT
                  "', not '%" PHYTO_STRING_FORMAT "'",
                  PHYTO_STRING_VIEW_PRINTF_ARGS(MskSyntaxKindName(kind)),
                  PHYTO_STRING_PRINTF_ARGS(tokens.data[0].text),
                  PHYTO_STRING_VIEW_PRINTF_ARGS(text));
      MskSyntaxTokensFree(&tokens);
#undef CLEANUP
    }
  }
  VEC_FREE(&all_kinds);
  TEST_PASS();
}
