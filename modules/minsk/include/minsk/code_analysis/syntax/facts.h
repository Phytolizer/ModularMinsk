#ifndef MINSK_CODE_ANALYSIS_SYNTAX_FACTS_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_FACTS_H_

#include <phyto/string/string.h>
#include <stdint.h>

#include "minsk/code_analysis/syntax/kind.h"

uint64_t MskSyntaxFactsBinaryOperatorPrecedence(MskSyntaxKind kind);
uint64_t MskSyntaxFactsUnaryOperatorPrecedence(MskSyntaxKind kind);
MskSyntaxKind MskSyntaxFactsKeywordKind(phyto_string_span_t text);
phyto_string_span_t MskSyntaxFactsGetText(MskSyntaxKind kind);
MskSyntaxKinds MskSyntaxFactsGetBinaryOperators(void);
MskSyntaxKinds MskSyntaxFactsGetUnaryOperators(void);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_FACTS_H_
