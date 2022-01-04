#include "minsk/code_analysis/syntax/name_expression.h"

MskNameExpressionSyntax* MskNameExpressionSyntaxNew(
    MskSyntaxToken identifier_token) {
  MskNameExpressionSyntax* syntax = calloc(1, sizeof(MskNameExpressionSyntax));
  syntax->base.base.cls = kMskSyntaxNodeClassExpression;
  syntax->base.cls = kMskSyntaxExpressionClassName;
  syntax->identifier_token = identifier_token;
  return syntax;
}

void MskNameExpressionSyntaxFree(MskNameExpressionSyntax* syntax) {
  MskSyntaxTokenFree(&syntax->identifier_token);
}
