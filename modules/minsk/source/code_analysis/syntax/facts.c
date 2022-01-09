#include "minsk/code_analysis/syntax/facts.h"

#include "minsk/code_analysis/syntax/kind.h"
#include "phyto/string/string.h"

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

MskSyntaxKind MskSyntaxFactsKeywordKind(phyto_string_span_t text) {
  if (phyto_string_span_equal(text, phyto_string_span_from_c("true"))) {
    return kMskSyntaxKindTrueKeyword;
  } else if (phyto_string_span_equal(text, phyto_string_span_from_c("false"))) {
    return kMskSyntaxKindFalseKeyword;
  } else {
    return kMskSyntaxKindIdentifierToken;
  }
}

phyto_string_span_t MskSyntaxFactsGetText(MskSyntaxKind kind) {
  switch (kind) {
    case kMskSyntaxKindPlusToken:
      return phyto_string_span_from_c("+");
    case kMskSyntaxKindMinusToken:
      return phyto_string_span_from_c("-");
    case kMskSyntaxKindStarToken:
      return phyto_string_span_from_c("*");
    case kMskSyntaxKindSlashToken:
      return phyto_string_span_from_c("/");
    case kMskSyntaxKindOpenParenthesisToken:
      return phyto_string_span_from_c("(");
    case kMskSyntaxKindCloseParenthesisToken:
      return phyto_string_span_from_c(")");
    case kMskSyntaxKindBangToken:
      return phyto_string_span_from_c("!");
    case kMskSyntaxKindBangEqualsToken:
      return phyto_string_span_from_c("!=");
    case kMskSyntaxKindEqualsToken:
      return phyto_string_span_from_c("=");
    case kMskSyntaxKindEqualsEqualsToken:
      return phyto_string_span_from_c("==");
    case kMskSyntaxKindAmpersandAmpersandToken:
      return phyto_string_span_from_c("&&");
    case kMskSyntaxKindPipePipeToken:
      return phyto_string_span_from_c("||");
    case kMskSyntaxKindTrueKeyword:
      return phyto_string_span_from_c("true");
    case kMskSyntaxKindFalseKeyword:
      return phyto_string_span_from_c("false");
    default:
      return phyto_string_span_empty();
  }
}

MskSyntaxKinds_t MskSyntaxFactsGetBinaryOperators(void) {
  MskSyntaxKinds_t all_kinds = MskSyntaxKindsGetAll();
  MskSyntaxKinds_t binary_operators =
      MskSyntaxKinds_init(&kMskSyntaxKindsCallbacks);
  for (uint64_t i = 0; i < all_kinds.size; i++) {
    MskSyntaxKind kind = all_kinds.data[i];
    if (MskSyntaxFactsBinaryOperatorPrecedence(kind) > 0) {
      MskSyntaxKinds_append(&binary_operators, kind);
    }
  }
  MskSyntaxKinds_free(&all_kinds);
  return binary_operators;
}

MskSyntaxKinds_t MskSyntaxFactsGetUnaryOperators(void) {
  MskSyntaxKinds_t all_kinds = MskSyntaxKindsGetAll();
  MskSyntaxKinds_t unary_operators = MskSyntaxKinds_init(&kMskSyntaxKindsCallbacks);
  for (uint64_t i = 0; i < all_kinds.size; i++) {
    MskSyntaxKind kind = all_kinds.data[i];
    if (MskSyntaxFactsUnaryOperatorPrecedence(kind) > 0) {
      MskSyntaxKinds_append(&unary_operators, kind);
    }
  }
  MskSyntaxKinds_free(&all_kinds);
  return unary_operators;
}
