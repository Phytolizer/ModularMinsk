#ifndef MINSK_CODE_ANALYSIS_SYNTAX_BINARY_EXPRESSION_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_BINARY_EXPRESSION_H_

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/token.h"

typedef struct {
  MskExpressionSyntax base;
  MskExpressionSyntax* left;
  MskSyntaxToken operator_token;
  MskExpressionSyntax* right;
} MskBinaryExpressionSyntax;

MskBinaryExpressionSyntax* MskBinaryExpressionSyntaxNew(
    MskExpressionSyntax* left,
    MskSyntaxToken operator_token,
    MskExpressionSyntax* right);
void MskBinaryExpressionSyntaxFree(MskBinaryExpressionSyntax* syntax);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_BINARY_EXPRESSION_H_
