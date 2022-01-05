#include "minsk/code_analysis/syntax/tree.h"

#include <stdint.h>
#include <stdlib.h>

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/code_analysis/syntax/lexer.h"
#include "minsk/code_analysis/syntax/parser.h"
#include "minsk/code_analysis/text/diagnostic_bag.h"

MskSyntaxTree MskSyntaxTreeParse(StringView text) {
  MskSyntaxParser parser = MskSyntaxParserNew(text);
  MskSyntaxTree tree = MskSyntaxParserParse(&parser);
  MskSyntaxParserFree(&parser);
  return tree;
}

MskSyntaxTokens MskSyntaxTreeParseTokens(StringView text) {
  MskSyntaxLexer lexer = MskNewSyntaxLexer(text);
  MskSyntaxTokens tokens = VEC_INIT_DEFAULT(MskSyntaxToken);
  while (true) {
    MskSyntaxToken token = MskSyntaxLexerLex(&lexer);
    if (token.kind == kMskSyntaxKindEndOfFileToken) {
      break;
    }
    VEC_PUSH(&tokens, token);
  }
  return tokens;
}

void MskSyntaxTreeFree(MskSyntaxTree* tree) {
  MskExpressionSyntaxFree(tree->root);
  free(tree->root);
  tree->root = NULL;
  MskSyntaxTokenFree(&tree->end_of_file_token);
  MskDiagnosticBagFree(&tree->diagnostics);
}
