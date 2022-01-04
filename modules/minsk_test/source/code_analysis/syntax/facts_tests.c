#include "minsk_test/code_analysis/syntax/facts_tests.h"

#include <minsk/code_analysis/syntax/facts.h>
#include <minsk/code_analysis/syntax/kind.h>
#include <minsk/code_analysis/syntax/token.h>
#include <minsk/code_analysis/syntax/tree.h>
#include <stddef.h>

static TEST_FUNC(GetTextRoundTrips);

static const MskSyntaxKind kSyntaxKinds[] = {
#define X(x) kMskSyntaxKind##x,
    MSK__SYNTAX_KINDS
#undef X
};

TEST_SUITE_FUNC(SyntaxFactsTests) {
  TEST_RUN(GetTextRoundTrips);
}

TEST_FUNC(GetTextRoundTrips) {
  for (size_t i = 0; i < sizeof(kSyntaxKinds) / sizeof(MskSyntaxKind); ++i) {
    MskSyntaxKind kind = kSyntaxKinds[i];
    StringView text = MskSyntaxFactsGetText(kind);
    if (StringViewSize(text) > 0) {
      MskSyntaxTokens tokens = MskSyntaxTreeParseTokens(text);
      TEST_ASSERT(tokens.size == 1, MskSyntaxTokensFree(&tokens),
                  "[%" STRING_VIEW_FMT "] Expected 1 token",
                  STRING_VIEW_PRINT(MskSyntaxKindName(kind)));
      TEST_ASSERT(tokens.data[0].kind == kind, MskSyntaxTokensFree(&tokens),
                  "[%" STRING_VIEW_FMT
                  "] Lexer returned kind %" STRING_VIEW_FMT,
                  STRING_VIEW_PRINT(MskSyntaxKindName(kind)),
                  STRING_VIEW_PRINT(MskSyntaxKindName(tokens.data[0].kind)));
      TEST_ASSERT(StringEqualView(tokens.data[0].text, text),
                  MskSyntaxTokensFree(&tokens),
                  "[%" STRING_VIEW_FMT "] Lexer returned text '%" STRING_FMT
                  "', not '%" STRING_VIEW_FMT "'",
                  STRING_VIEW_PRINT(MskSyntaxKindName(kind)),
                  STRING_PRINT(tokens.data[0].text), STRING_VIEW_PRINT(text));
      MskSyntaxTokensFree(&tokens);
    }
  }
  TEST_PASS();
}
