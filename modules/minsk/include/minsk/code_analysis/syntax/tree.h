#ifndef MINSK_CODE_ANALYSIS_SYNTAX_TREE_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_TREE_H_

#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/token.h"
#include "minsk/code_analysis/text/diagnostic_bag.h"

typedef struct {
  MskExpressionSyntax* root;
  MskSyntaxToken end_of_file_token;
  MskDiagnosticBag diagnostics;
} MskSyntaxTree;

MskSyntaxTree MskSyntaxTreeParse(StringView text);
MskSyntaxTokens MskSyntaxTreeParseTokens(StringView text);
void MskSyntaxTreeFree(MskSyntaxTree* tree);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_TREE_H_
