#ifndef MINSK_CODE_ANALYSIS_SYNTAX_NAME_EXPRESSION_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_NAME_EXPRESSION_H_

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/token.h"

typedef struct {
  MskExpressionSyntax base;
  MskSyntaxToken identifier_token;
} MskNameExpressionSyntax;

MskNameExpressionSyntax* MskNameExpressionSyntaxNew(
    MskSyntaxToken identifier_token);
void MskNameExpressionSyntaxFree(MskNameExpressionSyntax* syntax);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_NAME_EXPRESSION_H_
