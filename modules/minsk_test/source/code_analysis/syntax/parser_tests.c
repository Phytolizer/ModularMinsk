#include "minsk_test/code_analysis/syntax/parser_tests.h"

#include <phyto/collections/dynamic_array.h>
#include <phyto/string/string.h>
#include <stdint.h>
#include <vec/vec.h>

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/facts.h"
#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/code_analysis/syntax/node.h"
#include "minsk/code_analysis/syntax/token.h"

PHYTO_COLLECTIONS_DEQUE_IMPL(FlatSyntaxTree, MskSyntaxNode*);
PHYTO_COLLECTIONS_DYNAMIC_ARRAY_DECL(SyntaxNodeStack, MskSyntaxNode*);
PHYTO_COLLECTIONS_DYNAMIC_ARRAY_IMPL(SyntaxNodeStack, MskSyntaxNode*);

static void FreeExpressionNode(MskSyntaxNode* node) {
  MskExpressionSyntaxFree((MskExpressionSyntax*)node);
}

static void FreeTokenNode(MskSyntaxNode* node) {
  MskSyntaxTokenFree((MskSyntaxToken*)node);
}

static void MskSyntaxNodeFree(MskSyntaxNode** node) {
  switch ((*node)->cls) {
#define X(x)                   \
  case kMskSyntaxNodeClass##x: \
    Free##x##Node(*node);      \
    break;
    MSK__SYNTAX_NODE_CLASSES
#undef X
  }
  free(*node);
  *node = NULL;
}

static void MskSyntaxNodePrettyPrintNoColors(MskSyntaxNode* node, FILE* fp) {
  MskSyntaxNodePrettyPrint(node, fp, false);
}

// intentionally no free cb as we don't own the nodes
static const FlatSyntaxTree_callbacks_t kFlatSyntaxTreeCallbacks = {0};
// ditto for above
static const SyntaxNodeStack_callbacks_t kSyntaxNodeStackCallbacks = {0};

FlatSyntaxTree_t Flatten(MskSyntaxNode* node) {
  FlatSyntaxTree_t result = FlatSyntaxTree_new(10, &kFlatSyntaxTreeCallbacks);

  SyntaxNodeStack_t stack = SyntaxNodeStack_init(&kSyntaxNodeStackCallbacks);

  SyntaxNodeStack_append(&stack, node);

  while (stack.size > 0) {
    MskSyntaxNode* n = stack.data[stack.size - 1];
    stack.size--;
    FlatSyntaxTree_push_back(&result, n);
    MskSyntaxNodeChildren children = MskSyntaxNodeGetChildren(n);
    for (size_t i = 0; i < VEC_SIZE(&children); i++) {
      SyntaxNodeStack_append(&stack, children.data[i]);
    }
  }

  SyntaxNodeStack_free(&stack);
  return result;
}

typedef struct {
  // "tree"
  FlatSyntaxTree_t tree;
  size_t index;
} AssertingEnumerator;

static TEST_SUBTEST_FUNC(AssertToken,
                         AssertingEnumerator* enumerator,
                         MskSyntaxKind kind,
                         const char* text) {
  TEST_ASSERT(enumerator->index < enumerator->tree.count, (void)0,
              "no such node");
  MskSyntaxNode* node = enumerator->tree.data[enumerator->index];
  enumerator->index++;
  TEST_ASSERT(node->cls == kMskSyntaxNodeClassToken, (void)0, "Expected token");
  TEST_ASSERT(MskSyntaxNodeGetKind(node) == kind, (void)0,
              "Expected %" PHYTO_STRING_FORMAT ", not %" PHYTO_STRING_FORMAT,
              PHYTO_STRING_VIEW_PRINTF_ARGS(MskSyntaxKindName(kind)),
              PHYTO_STRING_VIEW_PRINTF_ARGS(
                  MskSyntaxKindName(MskSyntaxNodeGetKind(node))));
  phyto_string_span_t actual_text =
      phyto_string_as_span(((MskSyntaxToken*)node)->text);
  TEST_ASSERT(
      phyto_string_span_equal(actual_text, phyto_string_span_from_c(text)),
      (void)0, "Expected '%s', not '%" PHYTO_STRING_FORMAT "'", text,
      PHYTO_STRING_VIEW_PRINTF_ARGS(actual_text));
  TEST_SUBTEST_PASS();
}

static TEST_SUBTEST_FUNC(AssertNode,
                         AssertingEnumerator* enumerator,
                         MskSyntaxKind kind) {
  TEST_ASSERT(enumerator->index < enumerator->tree.count, (void)0,
              "no such node");
  MskSyntaxNode* node = enumerator->tree.data[enumerator->index];
  enumerator->index++;
  TEST_ASSERT(node->cls != kMskSyntaxNodeClassToken, (void)0,
              "Didn't expect a token here");
  TEST_ASSERT(MskSyntaxNodeGetKind(node) == kind, (void)0,
              "Expected %" PHYTO_STRING_FORMAT ", not %" PHYTO_STRING_FORMAT,
              PHYTO_STRING_VIEW_PRINTF_ARGS(MskSyntaxKindName(kind)),
              PHYTO_STRING_VIEW_PRINTF_ARGS(
                  MskSyntaxKindName(MskSyntaxNodeGetKind(node))));
  TEST_SUBTEST_PASS();
}

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
    phyto_string_span_t op1_text = MskSyntaxFactsGetText(pairs.data[i].op1);
    phyto_string_span_t op2_text = MskSyntaxFactsGetText(pairs.data[i].op2);
    phyto_string_t text = phyto_string_from_sprintf(
        "a %" PHYTO_STRING_FORMAT " b %" PHYTO_STRING_FORMAT " c",
        PHYTO_STRING_VIEW_PRINTF_ARGS(op1_text),
        PHYTO_STRING_VIEW_PRINTF_ARGS(op2_text));
    if (op1_precedence >= op2_precedence) {
    } else {
    }
    phyto_string_free(&text);
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
