#ifndef MINSK_CODE_ANALYSIS_SYNTAX_EXPRESSION_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_EXPRESSION_H_

#include "minsk/code_analysis/syntax/node.h"

#define MSK__EXPRESSION_KINDS \
  X(Binary)                   \
  X(Literal)                  \
  X(Parenthesized)

typedef enum {
#define X(x) kMskSyntaxExpressionKind##x,
  MSK__EXPRESSION_KINDS
#undef X
} MskExpressionSyntaxKind;

typedef struct {
  MskSyntaxNode base;
  MskExpressionSyntaxKind kind;
} MskExpressionSyntax;

void MskExpressionSyntaxFree(MskExpressionSyntax* syntax);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_EXPRESSION_H_
