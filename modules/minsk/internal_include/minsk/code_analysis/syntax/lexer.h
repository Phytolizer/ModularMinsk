#ifndef MINSK_CODE_ANALYSIS_SYNTAX_LEXER_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_LEXER_H_

#include <phyto/string/string.h>
#include <stdint.h>

#include "minsk/code_analysis/syntax/token.h"
#include "minsk/code_analysis/text/diagnostic_bag.h"

typedef struct {
  phyto_string_span_t text;
  uint64_t position;
  MskDiagnosticBag_t diagnostics;
} MskSyntaxLexer;

MskSyntaxLexer MskNewSyntaxLexer(phyto_string_span_t text);
MskSyntaxToken MskSyntaxLexerLex(MskSyntaxLexer* lexer);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_LEXER_H_
