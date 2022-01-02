#ifndef MINSK_CODE_ANALYSIS_SYNTAX_PARSER_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_PARSER_H_

#include <stdint.h>
#include <string/string.h>
#include <vec/vec.h>

#include "minsk/code_analysis/syntax/token.h"

typedef VEC_TYPE(MskSyntaxToken) MskSyntaxTokens;

typedef struct {
  MskSyntaxTokens tokens;
  uint64_t position;
} MskSyntaxParser;

MskSyntaxParser MskSyntaxParserNew(StringView text);
void MskSyntaxParserFree(MskSyntaxParser* parser);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_PARSER_H_