#ifndef MINSK_CODE_ANALYSIS_SYNTAX_TREE_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_TREE_H_

#include "minsk/code_analysis/text/diagnostics.h"
#include "minsk/code_analysis/syntax/expression.h"
#include "minsk/code_analysis/syntax/token.h"

typedef struct {
  MskExpressionSyntax* root;
  MskSyntaxToken end_of_file_token;
  MskDiagnostics diagnostics;
} MskSyntaxTree;

MskSyntaxTree MskSyntaxTreeParse(StringView text);
void MskSyntaxTreeFree(MskSyntaxTree* tree);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_TREE_H_
