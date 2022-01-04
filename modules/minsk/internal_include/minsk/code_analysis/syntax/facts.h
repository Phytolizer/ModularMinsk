#ifndef MINSK_CODE_ANALYSIS_SYNTAX_FACTS_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_FACTS_H_

#include <stdint.h>

#include "minsk/code_analysis/syntax/kind.h"

uint64_t MskSyntaxFactsBinaryOperatorPrecedence(MskSyntaxKind kind);
uint64_t MskSyntaxFactsUnaryOperatorPrecedence(MskSyntaxKind kind);
MskSyntaxKind MskSyntaxFactsKeywordKind(StringView text);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_FACTS_H_
