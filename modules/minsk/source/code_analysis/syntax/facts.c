#include "minsk_private/code_analysis/syntax/facts.h"

#include "minsk/code_analysis/syntax/kind.h"

uint64_t MskSyntaxFactsBinaryOperatorPrecedence(MskSyntaxKind kind) {
  switch (kind) {
    case kMskSyntaxKindStarToken:
    case kMskSyntaxKindSlashToken:
      return 2;
    case kMskSyntaxKindPlusToken:
    case kMskSyntaxKindMinusToken:
      return 1;
    default:
      return 0;
  }
}
