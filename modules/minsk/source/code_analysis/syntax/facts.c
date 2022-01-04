#include "minsk/code_analysis/syntax/facts.h"

#include "minsk/code_analysis/syntax/kind.h"

uint64_t MskSyntaxFactsBinaryOperatorPrecedence(MskSyntaxKind kind) {
  switch (kind) {
    case kMskSyntaxKindStarToken:
    case kMskSyntaxKindSlashToken:
      return 5;
    case kMskSyntaxKindPlusToken:
    case kMskSyntaxKindMinusToken:
      return 4;
    case kMskSyntaxKindEqualsEqualsToken:
    case kMskSyntaxKindBangEqualsToken:
      return 3;
    case kMskSyntaxKindAmpersandAmpersandToken:
      return 2;
    case kMskSyntaxKindPipePipeToken:
      return 1;
    default:
      return 0;
  }
}

uint64_t MskSyntaxFactsUnaryOperatorPrecedence(MskSyntaxKind kind) {
  switch (kind) {
    case kMskSyntaxKindPlusToken:
    case kMskSyntaxKindMinusToken:
    case kMskSyntaxKindBangToken:
      return 6;
    default:
      return 0;
  }
}

MskSyntaxKind MskSyntaxFactsKeywordKind(StringView text) {
  if (StringViewEqualC(text, "true")) {
    return kMskSyntaxKindTrueKeyword;
  } else if (StringViewEqualC(text, "false")) {
    return kMskSyntaxKindFalseKeyword;
  } else {
    return kMskSyntaxKindIdentifierToken;
  }
}
