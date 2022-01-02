#include "minsk/code_analysis/syntax/parenthesized_expression.h"

#include <stdlib.h>

MskParenthesizedExpressionSyntax* MskParenthesizedExpressionSyntaxNew(
    MskSyntaxToken open_parenthesis_token,
    MskExpressionSyntax* expression,
    MskSyntaxToken close_parenthesis_token) {
  MskParenthesizedExpressionSyntax* syntax =
      calloc(1, sizeof(MskParenthesizedExpressionSyntax));
  syntax->base.base.kind = kMskSyntaxNodeKindExpression;
  syntax->base.kind = kMskSyntaxExpressionKindParenthesized;
  syntax->open_parenthesis_token = open_parenthesis_token;
  syntax->expression = expression;
  syntax->close_parenthesis_token = close_parenthesis_token;
  return syntax;
}
