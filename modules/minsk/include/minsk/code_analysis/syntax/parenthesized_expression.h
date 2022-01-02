#ifndef MINSK_CODE_ANALYSIS_SYNTAX_PARENTHESIZED_EXPRESSION_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_PARENTHESIZED_EXPRESSION_H_

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/token.h"

typedef struct {
  MskExpressionSyntax base;
  MskSyntaxToken open_parenthesis_token;
  MskExpressionSyntax* expression;
  MskSyntaxToken close_parenthesis_token;
} MskParenthesizedExpressionSyntax;

MskParenthesizedExpressionSyntax* MskParenthesizedExpressionSyntaxNew(
    MskSyntaxToken open_parenthesis_token,
    MskExpressionSyntax* expression,
    MskSyntaxToken close_parenthesis_token);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_PARENTHESIZED_EXPRESSION_H_
