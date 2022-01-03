#ifndef MINSK_CODE_ANALYSIS_SYNTAX_LEXER_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_LEXER_H_

#include <stdint.h>
#include <string/string.h>

#include "minsk/code_analysis/syntax/token.h"
#include "minsk/code_analysis/text/diagnostic_bag.h"

typedef struct {
  StringView text;
  uint64_t position;
  MskDiagnosticBag diagnostics;
} MskSyntaxLexer;

MskSyntaxLexer MskNewSyntaxLexer(StringView text);
MskSyntaxToken MskSyntaxLexerLex(MskSyntaxLexer* lexer);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_LEXER_H_
