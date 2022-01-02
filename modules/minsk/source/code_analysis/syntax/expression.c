#include "minsk/code_analysis/syntax/expression.h"

#include "minsk/code_analysis/syntax/binary_expression.h"
#include "minsk/code_analysis/syntax/literal_expression.h"

void MskExpressionSyntaxFree(MskExpressionSyntax* syntax) {
  switch (syntax->cls) {
    case kMskSyntaxExpressionClassLiteral:
      MskLiteralExpressionSyntaxFree((MskLiteralExpressionSyntax*)syntax);
      break;
    case kMskSyntaxExpressionClassBinary:
      MskBinaryExpressionSyntaxFree((MskBinaryExpressionSyntax*)syntax);
      break;
    default:
      break;
  }
}
