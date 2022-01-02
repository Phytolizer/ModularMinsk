#include "minsk/code_analysis/syntax/unary_expression.h"

MskUnaryExpressionSyntax* MskUnaryExpressionSyntaxNew(
    MskSyntaxToken operator_token,
    MskExpressionSyntax* operand) {
  MskUnaryExpressionSyntax* syntax =
      calloc(1, sizeof(MskUnaryExpressionSyntax));
  syntax->base.base.kind = kMskSyntaxNodeKindExpression;
  syntax->base.kind = kMskSyntaxExpressionKindUnary;
  syntax->operator_token = operator_token;
  syntax->operand = operand;
  return syntax;
}
