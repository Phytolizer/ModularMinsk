#include <ansi_esc/ansi_esc.h>
#include <inttypes.h>
#include <minsk/code_analysis/evaluator.h>
#include <minsk/code_analysis/syntax/expression.h>
#include <minsk/code_analysis/syntax/lexer.h>
#include <minsk/code_analysis/syntax/node.h>
#include <minsk/code_analysis/syntax/parser.h>
#include <minsk/code_analysis/syntax/token.h>
#include <minsk/code_analysis/syntax/tree.h>
#include <minsk/runtime/object.h>
#include <stdio.h>
#include <stdlib.h>
#include <string/string.h>

int main(void) {
  while (true) {
    printf("minsk> ");
    fflush(stdout);
    String text = StringGetLine(stdin);
    if (text.size == 0) {
      // EOF condition
      // Print a newline so the prompt isn't on the same line as the Bash
      // prompt.
      printf("\n");
      break;
    }

    MskSyntaxTree syntax_tree = MskSyntaxTreeParse(StringAsView(text));
    MskSyntaxNodePrettyPrint(&syntax_tree.root->base, stdout, true);
    if (syntax_tree.diagnostics.size > 0) {
      printf("\n");
      printf(ANSI_ESC_DIM ANSI_ESC_FG_RED);
      for (size_t i = 0; i < syntax_tree.diagnostics.size; ++i) {
        String diagnostic = syntax_tree.diagnostics.data[i];
        printf("%" STRING_FMT "\n", STRING_PRINT(diagnostic));
      }
      printf(ANSI_ESC_RESET);
    } else {
      MskEvaluator evaluator = MskEvaluatorNew(syntax_tree.root);
      MskRuntimeObject result = MskEvaluatorEvaluate(&evaluator);
      MskRuntimeObjectPrint(&result, stdout);
      printf("\n");
    }
    MskSyntaxTreeFree(&syntax_tree);
    StringFree(&text);
  }
}
