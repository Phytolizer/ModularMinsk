#include "minsk/code_analysis/syntax/tree.h"

#include <stdint.h>
#include <stdlib.h>

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk_private/code_analysis/syntax/parser.h"

MskSyntaxTree MskSyntaxTreeParse(StringView text) {
  MskSyntaxParser parser = MskSyntaxParserNew(text);
  MskSyntaxTree tree = MskSyntaxParserParse(&parser);
  MskSyntaxParserFree(&parser);
  return tree;
}

void MskSyntaxTreeFree(MskSyntaxTree* tree) {
  MskExpressionSyntaxFree(tree->root);
  tree->root = NULL;
  MskSyntaxTokenFree(&tree->end_of_file_token);
  for (uint64_t i = 0; i < tree->diagnostics.size; ++i) {
    StringFree(&tree->diagnostics.data[i]);
  }
  VEC_FREE(&tree->diagnostics);
}
