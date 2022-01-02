#ifndef MINSK_CODE_ANALYSIS_SYNTAX_PARSER_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_PARSER_H_

#include <stdint.h>
#include <string/string.h>
#include <vec/vec.h>

#include "minsk/code_analysis/diagnostics.h"
#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/token.h"
#include "minsk/code_analysis/syntax/tree.h"

typedef VEC_TYPE(MskSyntaxToken) MskSyntaxTokens;

typedef struct {
  MskSyntaxTokens tokens;
  uint64_t position;
  MskDiagnostics diagnostics;
} MskSyntaxParser;

MskSyntaxParser MskSyntaxParserNew(StringView text);
void MskSyntaxParserFree(MskSyntaxParser* parser);
MskSyntaxTree MskSyntaxParserParse(MskSyntaxParser* parser);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_PARSER_H_
