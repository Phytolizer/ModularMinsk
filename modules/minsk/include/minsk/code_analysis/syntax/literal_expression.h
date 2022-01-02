#ifndef MINSK_CODE_ANALYSIS_SYNTAX_LITERAL_EXPRESSION_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_LITERAL_EXPRESSION_H_

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/token.h"

typedef struct {
  MskExpressionSyntax base;
  MskSyntaxToken literal_token;
} MskLiteralExpressionSyntax;

MskLiteralExpressionSyntax* MskLiteralExpressionSyntaxNew(
    MskSyntaxToken literal_token);
void MskLiteralExpressionSyntaxFree(MskLiteralExpressionSyntax* syntax);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_LITERAL_EXPRESSION_H_
