#include "minsk/code_analysis/syntax/literal_expression.h"

#include <stdlib.h>

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/node.h"
#include "minsk/code_analysis/syntax/token.h"

MskLiteralExpressionSyntax* MskLiteralExpressionSyntaxNew(
    MskSyntaxToken literal_token) {
  MskLiteralExpressionSyntax* syntax =
      calloc(1, sizeof(MskLiteralExpressionSyntax));
  syntax->base.base.cls = kMskSyntaxNodeClassExpression;
  syntax->base.cls = kMskSyntaxExpressionClassLiteral;
  syntax->literal_token = literal_token;
  return syntax;
}

void MskLiteralExpressionSyntaxFree(MskLiteralExpressionSyntax* syntax) {
  MskSyntaxTokenFree(&syntax->literal_token);
}
