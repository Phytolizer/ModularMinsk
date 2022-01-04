#include "minsk/code_analysis/syntax/unary_expression.h"

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/node.h"

MskUnaryExpressionSyntax* MskUnaryExpressionSyntaxNew(
    MskSyntaxToken operator_token,
    MskExpressionSyntax* operand) {
  MskUnaryExpressionSyntax* syntax =
      calloc(1, sizeof(MskUnaryExpressionSyntax));
  syntax->base.base.cls = kMskSyntaxNodeClassExpression;
  syntax->base.cls = kMskSyntaxExpressionClassUnary;
  syntax->operator_token = operator_token;
  syntax->operand = operand;
  return syntax;
}

void MskUnaryExpressionSyntaxFree(MskUnaryExpressionSyntax* syntax) {
  MskSyntaxTokenFree(&syntax->operator_token);
  MskExpressionSyntaxFree(syntax->operand);
  free(syntax->operand);
}
