#include "minsk/code_analysis/syntax/tree.h"

#include <phyto/string/string.h>
#include <stdint.h>
#include <stdlib.h>

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/code_analysis/syntax/lexer.h"
#include "minsk/code_analysis/syntax/parser.h"
#include "minsk/code_analysis/text/diagnostic_bag.h"

MskSyntaxTree MskSyntaxTreeParse(phyto_string_span_t text) {
  MskSyntaxParser parser = MskSyntaxParserNew(text);
  MskSyntaxTree tree = MskSyntaxParserParse(&parser);
  MskSyntaxParserFree(&parser);
  return tree;
}

MskSyntaxTokens_t MskSyntaxTreeParseTokens(phyto_string_span_t text) {
  MskSyntaxLexer lexer = MskNewSyntaxLexer(text);
  MskSyntaxTokens_t tokens = MskSyntaxTokens_init(&kMskSyntaxTokensCallbacks);
  while (true) {
    MskSyntaxToken token = MskSyntaxLexerLex(&lexer);
    if (token.kind == kMskSyntaxKindEndOfFileToken) {
      break;
    }
    MskSyntaxTokens_append(&tokens, token);
  }
  return tokens;
}

void MskSyntaxTreeFree(MskSyntaxTree* tree) {
  MskExpressionSyntaxFree(tree->root);
  free(tree->root);
  tree->root = NULL;
  MskSyntaxTokenFree(&tree->end_of_file_token);
  MskDiagnosticBag_free(&tree->diagnostics);
}
