#include "minsk/code_analysis/syntax/parser.h"

#include <stdlib.h>

#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/code_analysis/syntax/lexer.h"

MskSyntaxParser MskSyntaxParserNew(StringView text) {
  MskLexer lexer = MskNewLexer(text);
  MskSyntaxParser parser = {0};
  while (true) {
    MskSyntaxToken token = MskLexerLex(&lexer);
    if (token.kind != kMskSyntaxKindBadToken &&
        token.kind != kMskSyntaxKindWhitespaceToken) {
      VEC_PUSH(&parser.tokens, token);
    }
    if (token.kind == kMskSyntaxKindEndOfFileToken) {
      break;
    }
  }
  return parser;
}

void MskSyntaxParserFree(MskSyntaxParser* parser) {
  for (size_t i = 0; i < parser->tokens.size; ++i) {
    MskSyntaxTokenFree(&parser->tokens.data[i]);
  }
  VEC_FREE(&parser->tokens);
}
