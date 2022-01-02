#include "minsk/code_analysis/syntax/expression.h"

#include "minsk/code_analysis/syntax/binary_expression.h"
#include "minsk/code_analysis/syntax/literal_expression.h"

void MskExpressionSyntaxFree(MskExpressionSyntax* syntax) {
  switch (syntax->kind) {
    case kMskSyntaxExpressionKindLiteral:
      MskLiteralExpressionSyntaxFree((MskLiteralExpressionSyntax*)syntax);
      break;
    case kMskSyntaxExpressionKindBinary:
      MskBinaryExpressionSyntaxFree((MskBinaryExpressionSyntax*)syntax);
      break;
    default:
      break;
  }
}
