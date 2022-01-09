#ifndef MINSK_CODE_ANALYSIS_SYNTAX_TREE_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_TREE_H_

#include <phyto/string/string.h>

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/token.h"
#include "minsk/code_analysis/text/diagnostic_bag.h"

typedef struct {
  MskExpressionSyntax* root;
  MskSyntaxToken end_of_file_token;
  MskDiagnosticBag_t diagnostics;
} MskSyntaxTree;

MskSyntaxTree MskSyntaxTreeParse(phyto_string_span_t text);
MskSyntaxTokens_t MskSyntaxTreeParseTokens(phyto_string_span_t text);
void MskSyntaxTreeFree(MskSyntaxTree* tree);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_TREE_H_
