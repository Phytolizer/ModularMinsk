#ifndef MINSK_CODE_ANALYSIS_SYNTAX_ASSIGNMENT_EXPRESSION_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_ASSIGNMENT_EXPRESSION_H_

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/token.h"
typedef struct {
  MskExpressionSyntax base;
  MskSyntaxToken identifier_token;
  MskSyntaxToken equals_token;
  MskExpressionSyntax* expression;
} MskAssignmentExpressionSyntax;

MskAssignmentExpressionSyntax* MskAssignmentExpressionSyntaxNew(
    MskSyntaxToken identifier_token,
    MskSyntaxToken equals_token,
    MskExpressionSyntax* expression);
void MskAssignmentExpressionSyntaxFree(MskAssignmentExpressionSyntax* syntax);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_ASSIGNMENT_EXPRESSION_H_
