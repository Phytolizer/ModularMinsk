#ifndef MINSK_CODE_ANALYSIS_SYNTAX_UNARY_EXPRESSION_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_UNARY_EXPRESSION_H_

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/token.h"

typedef struct {
  MskExpressionSyntax base;
  MskSyntaxToken operator_token;
  MskExpressionSyntax* operand;
} MskUnaryExpressionSyntax;

MskUnaryExpressionSyntax* MskUnaryExpressionSyntaxNew(
    MskSyntaxToken operator_token,
    MskExpressionSyntax* operand);
void MskUnaryExpressionSyntaxFree(MskUnaryExpressionSyntax* syntax);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_UNARY_EXPRESSION_H_
