#include "minsk/code_analysis/syntax/node.h"

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/kind.h"

const char* const kMskSyntaxKindNames[] = {
#define X(x) [kMskSyntaxNodeKind##x] = #x,
    MSK__SYNTAX_NODES
#undef X
};

StringView MskSyntaxNodeKindName(MskSyntaxNodeKind kind) {
  return StringViewFromC(kMskSyntaxKindNames[kind]);
}

static MskSyntaxKind GetExpressionKind(MskSyntaxNode* node);
static MskSyntaxKind GetBinaryExpressionKind(MskExpressionSyntax* syntax);
static MskSyntaxKind GetLiteralExpressionKind(MskExpressionSyntax* syntax);

MskSyntaxKind MskSyntaxNodeGetKind(MskSyntaxNode* node) {
  switch (node->kind) {
#define X(x)                  \
  case kMskSyntaxNodeKind##x: \
    return Get##x##Kind(node);
    MSK__SYNTAX_NODES
#undef X
    default:
      return kMskSyntaxKindInvalid;
  }
}

MskSyntaxKind GetExpressionKind(MskSyntaxNode* node) {
  MskExpressionSyntax* syntax = (MskExpressionSyntax*)node;
  switch (syntax->kind) {
#define X(x)                        \
  case kMskSyntaxExpressionKind##x: \
    return Get##x##ExpressionKind(syntax);
    MSK__EXPRESSION_KINDS
#undef X
    default:
      return kMskSyntaxKindInvalid;
  }
}

MskSyntaxKind GetBinaryExpressionKind(MskExpressionSyntax* syntax) {
  (void)syntax;
  return kMskSyntaxKindBinaryExpression;
}

MskSyntaxKind GetLiteralExpressionKind(MskExpressionSyntax* syntax) {
  (void)syntax;
  return kMskSyntaxKindLiteralExpression;
}
