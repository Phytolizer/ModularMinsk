#include "minsk/code_analysis/syntax/binary_expression.h"

#include <stdlib.h>

MskBinaryExpressionSyntax* MskBinaryExpressionSyntaxNew(
    MskExpressionSyntax* left,
    MskSyntaxToken operator_token,
    MskExpressionSyntax* right) {
  MskBinaryExpressionSyntax* syntax =
      calloc(1, sizeof(MskBinaryExpressionSyntax));
  syntax->base.base.cls = kMskSyntaxNodeClassExpression;
  syntax->base.cls = kMskSyntaxExpressionClassBinary;
  syntax->left = left;
  syntax->operator_token = operator_token;
  syntax->right = right;
  return syntax;
}

void MskBinaryExpressionSyntaxFree(MskBinaryExpressionSyntax* syntax) {
  MskExpressionSyntaxFree(syntax->left);
  free(syntax->left);
  MskExpressionSyntaxFree(syntax->right);
  free(syntax->right);
  MskSyntaxTokenFree(&syntax->operator_token);
}
