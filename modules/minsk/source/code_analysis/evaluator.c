#include "minsk/code_analysis/evaluator.h"

#include <assert.h>
#include <stdio.h>

#include "minsk/code_analysis/syntax/binary_expression.h"
#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/literal_expression.h"
#include "minsk/code_analysis/syntax/parenthesized_expression.h"
#include "minsk/runtime/object.h"

static MskRuntimeObject EvaluateExpression(MskExpressionSyntax* expression);
static MskRuntimeObject EvaluateLiteralExpression(
    MskLiteralExpressionSyntax* expression);
static MskRuntimeObject EvaluateBinaryExpression(
    MskBinaryExpressionSyntax* expression);
static MskRuntimeObject EvaluateParenthesizedExpression(
    MskParenthesizedExpressionSyntax* expression);

MskEvaluator MskEvaluatorNew(MskExpressionSyntax* root) {
  return (MskEvaluator){
      .root = root,
  };
}

MskRuntimeObject MskEvaluatorEvaluate(MskEvaluator* evaluator) {
  return EvaluateExpression(evaluator->root);
}

MskRuntimeObject EvaluateExpression(MskExpressionSyntax* expression) {
  switch (expression->kind) {
    case kMskSyntaxExpressionKindLiteral:
      return EvaluateLiteralExpression((MskLiteralExpressionSyntax*)expression);
    case kMskSyntaxExpressionKindBinary:
      return EvaluateBinaryExpression((MskBinaryExpressionSyntax*)expression);
    case kMskSyntaxExpressionKindParenthesized:
      return EvaluateParenthesizedExpression(
          (MskParenthesizedExpressionSyntax*)expression);
    default:
      return (MskRuntimeObject){0};
  }
}

MskRuntimeObject EvaluateLiteralExpression(
    MskLiteralExpressionSyntax* expression) {
  return expression->literal_token.value;
}

MskRuntimeObject EvaluateBinaryExpression(
    MskBinaryExpressionSyntax* expression) {
  MskRuntimeObject left = EvaluateExpression(expression->left);
  MskRuntimeObject right = EvaluateExpression(expression->right);
  assert(left.kind == kMskObjectKindInteger &&
         right.kind == kMskObjectKindInteger && "invalid operands");
  switch (expression->operator_token.kind) {
    case kMskSyntaxKindPlusToken:
      return MskRuntimeObjectNewInteger(left.value.integer +
                                        right.value.integer);
    case kMskSyntaxKindMinusToken:
      return MskRuntimeObjectNewInteger(left.value.integer -
                                        right.value.integer);
    case kMskSyntaxKindStarToken:
      return MskRuntimeObjectNewInteger(left.value.integer *
                                        right.value.integer);
    case kMskSyntaxKindSlashToken:
      return MskRuntimeObjectNewInteger(left.value.integer /
                                        right.value.integer);
    default:
      fprintf(stderr, "Unexpected binary operator %" STRING_VIEW_FMT "\n",
              STRING_VIEW_PRINT(
                  MskSyntaxKindName(expression->operator_token.kind)));
      assert(false);
  }
}

MskRuntimeObject EvaluateParenthesizedExpression(
    MskParenthesizedExpressionSyntax* expression) {
  return EvaluateExpression(expression->expression);
}
