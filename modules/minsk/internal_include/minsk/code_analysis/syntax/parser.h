#ifndef MINSK_CODE_ANALYSIS_SYNTAX_PARSER_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_PARSER_H_

#include <phyto/string/string.h>
#include <stdint.h>

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/token.h"
#include "minsk/code_analysis/syntax/tree.h"
#include "minsk/code_analysis/text/diagnostic_bag.h"

typedef struct {
  MskSyntaxTokens_t tokens;
  uint64_t position;
  MskDiagnosticBag_t diagnostics;
} MskSyntaxParser;

#define MSK_SYNTAX_PARSER_INIT                                                 \
  (MskSyntaxParser) {                                                          \
    .tokens = MskSyntaxTokens_init(&kMskSyntaxTokensCallbacks), .position = 0, \
    .diagnostics = MskDiagnosticBag_init(&kMskDiagnosticBagCallbacks),         \
  }

MskSyntaxParser MskSyntaxParserNew(phyto_string_span_t text);
void MskSyntaxParserFree(MskSyntaxParser* parser);
MskSyntaxTree MskSyntaxParserParse(MskSyntaxParser* parser);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_PARSER_H_
