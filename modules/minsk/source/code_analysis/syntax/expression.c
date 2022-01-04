#include "minsk/code_analysis/syntax/expression.h"

#include "minsk/code_analysis/syntax/binary_expression.h"
#include "minsk/code_analysis/syntax/literal_expression.h"
#include "minsk/code_analysis/syntax/name_expression.h"
#include "minsk/code_analysis/syntax/parenthesized_expression.h"
#include "minsk/code_analysis/syntax/unary_expression.h"

void MskExpressionSyntaxFree(MskExpressionSyntax* syntax) {
  switch (syntax->cls) {
    case kMskSyntaxExpressionClassLiteral:
      MskLiteralExpressionSyntaxFree((MskLiteralExpressionSyntax*)syntax);
      break;
    case kMskSyntaxExpressionClassBinary:
      MskBinaryExpressionSyntaxFree((MskBinaryExpressionSyntax*)syntax);
      break;
    case kMskSyntaxExpressionClassParenthesized:
      MskParenthesizedExpressionSyntaxFree(
          (MskParenthesizedExpressionSyntax*)syntax);
      break;
    case kMskSyntaxExpressionClassName:
      MskNameExpressionSyntaxFree((MskNameExpressionSyntax*)syntax);
      break;
    case kMskSyntaxExpressionClassUnary:
      MskUnaryExpressionSyntaxFree((MskUnaryExpressionSyntax*)syntax);
      break;
    default:
      break;
  }
}
