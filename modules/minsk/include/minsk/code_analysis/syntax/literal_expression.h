#ifndef MINSK_CODE_ANALYSIS_SYNTAX_LITERAL_EXPRESSION_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_LITERAL_EXPRESSION_H_

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/token.h"
#include "minsk/runtime/object.h"

typedef struct {
  MskExpressionSyntax base;
  MskSyntaxToken literal_token;
  MskRuntimeObject value;
} MskLiteralExpressionSyntax;

MskLiteralExpressionSyntax* MskLiteralExpressionSyntaxNew(
    MskSyntaxToken literal_token, MskRuntimeObject value);
void MskLiteralExpressionSyntaxFree(MskLiteralExpressionSyntax* syntax);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_LITERAL_EXPRESSION_H_
