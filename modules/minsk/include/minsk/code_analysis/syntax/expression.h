#ifndef MINSK_CODE_ANALYSIS_SYNTAX_EXPRESSION_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_EXPRESSION_H_

#include "minsk/code_analysis/syntax/node.h"

#define MSK__EXPRESSION_CLASSES \
  X(Binary)                     \
  X(Literal)                    \
  X(Parenthesized)              \
  X(Unary)                      \
  X(Name)                       \
  X(Assignment)

typedef enum {
#define X(x) kMskSyntaxExpressionClass##x,
  MSK__EXPRESSION_CLASSES
#undef X
} MskExpressionSyntaxClass;

typedef struct {
  MskSyntaxNode base;
  MskExpressionSyntaxClass cls;
} MskExpressionSyntax;

void MskExpressionSyntaxFree(MskExpressionSyntax* syntax);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_EXPRESSION_H_
