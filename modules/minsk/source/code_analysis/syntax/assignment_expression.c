#include "minsk/code_analysis/syntax/assignment_expression.h"
MskAssignmentExpressionSyntax* MskAssignmentExpressionSyntaxNew(
    MskSyntaxToken identifier_token,
    MskSyntaxToken equals_token,
    MskExpressionSyntax* expression) {
  MskAssignmentExpressionSyntax* syntax =
      calloc(1, sizeof(MskAssignmentExpressionSyntax));
  syntax->base.base.cls = kMskSyntaxNodeClassExpression;
  syntax->base.cls = kMskSyntaxExpressionClassAssignment;
  syntax->identifier_token = identifier_token;
  syntax->equals_token = equals_token;
  syntax->expression = expression;
  return syntax;
}

void MskAssignmentExpressionSyntaxFree(MskAssignmentExpressionSyntax* syntax)
{
  MskSyntaxTokenFree(&syntax->identifier_token);
  MskSyntaxTokenFree(&syntax->equals_token);
  MskExpressionSyntaxFree(syntax->expression);
  free(syntax->expression);
}
