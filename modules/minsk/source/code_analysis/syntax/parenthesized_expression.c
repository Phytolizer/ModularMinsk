#include "minsk/code_analysis/syntax/parenthesized_expression.h"

#include <stdlib.h>

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/node.h"
#include "minsk/code_analysis/syntax/token.h"

MskParenthesizedExpressionSyntax* MskParenthesizedExpressionSyntaxNew(
    MskSyntaxToken open_parenthesis_token,
    MskExpressionSyntax* expression,
    MskSyntaxToken close_parenthesis_token) {
  MskParenthesizedExpressionSyntax* syntax =
      calloc(1, sizeof(MskParenthesizedExpressionSyntax));
  syntax->base.base.cls = kMskSyntaxNodeClassExpression;
  syntax->base.cls = kMskSyntaxExpressionClassParenthesized;
  syntax->open_parenthesis_token = open_parenthesis_token;
  syntax->expression = expression;
  syntax->close_parenthesis_token = close_parenthesis_token;
  return syntax;
}

void MskParenthesizedExpressionSyntaxFree(
    MskParenthesizedExpressionSyntax* syntax) {
  MskSyntaxTokenFree(&syntax->open_parenthesis_token);
  MskExpressionSyntaxFree(syntax->expression);
  free(syntax->expression);
  MskSyntaxTokenFree(&syntax->close_parenthesis_token);
}
