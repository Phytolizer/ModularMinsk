#include "minsk_test/code_analysis/syntax/parser_tests.h"

#include <stdint.h>

#include "minsk/code_analysis/syntax/facts.h"
#include "minsk/code_analysis/syntax/kind.h"
#include "string/string.h"
#include "vec/vec.h"

typedef struct {
  MskSyntaxKind op1;
  MskSyntaxKind op2;
} SyntaxKindPair;

typedef VEC_TYPE(SyntaxKindPair) SyntaxKindPairs;

static TEST_FUNC(BinaryExpressionHonorsPrecedences);

static SyntaxKindPairs GetBinaryOperatorPairs(void);

TEST_SUITE_FUNC(ParserTests) {
  TEST_RUN(BinaryExpressionHonorsPrecedences);
}

TEST_FUNC(BinaryExpressionHonorsPrecedences) {
  SyntaxKindPairs pairs = GetBinaryOperatorPairs();
  for (uint64_t i = 0; i < VEC_SIZE(&pairs); ++i) {
    uint64_t op1_precedence =
        MskSyntaxFactsBinaryOperatorPrecedence(pairs.data[i].op1);
    uint64_t op2_precedence =
        MskSyntaxFactsBinaryOperatorPrecedence(pairs.data[i].op2);
    StringView op1_text = MskSyntaxFactsGetText(pairs.data[i].op1);
    StringView op2_text = MskSyntaxFactsGetText(pairs.data[i].op2);
    String text =
        StringFormat("a %" STRING_VIEW_FMT " b %" STRING_VIEW_FMT " c",
                     STRING_VIEW_PRINT(op1_text), STRING_VIEW_PRINT(op2_text));
    if (op1_precedence >= op2_precedence) {
    } else {
    }
    StringFree(&text);
  }
  VEC_FREE(&pairs);
  TEST_PASS();
}

SyntaxKindPairs GetBinaryOperatorPairs(void) {
  MskSyntaxKinds ops = MskSyntaxFactsGetBinaryOperators();
  SyntaxKindPairs pairs = VEC_INIT_DEFAULT(SyntaxKindPair);
  for (size_t i = 0; i < VEC_SIZE(&ops); ++i) {
    MskSyntaxKind op1 = ops.data[i];
    for (size_t j = i + 1; j < VEC_SIZE(&ops); ++j) {
      MskSyntaxKind op2 = ops.data[j];
      VEC_PUSH(&pairs, ((SyntaxKindPair){op1, op2}));
    }
  }
  VEC_FREE(&ops);
  return pairs;
}
