#include <inttypes.h>
#include <minsk/code_analysis/syntax/expression.h>
#include <minsk/code_analysis/syntax/lexer.h>
#include <minsk/code_analysis/syntax/node.h>
#include <minsk/code_analysis/syntax/parser.h>
#include <minsk/code_analysis/syntax/token.h>
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

    MskSyntaxParser parser = MskSyntaxParserNew(StringAsView(text));
    MskExpressionSyntax* syntax_tree = MskSyntaxParserParse(&parser);
    MskSyntaxNodePrettyPrint(&syntax_tree->base, stdout, true);
    if (parser.diagnostics.size > 0) {
      printf("\n");
      printf("\x1b[2;31m");
      for (size_t i = 0; i < parser.diagnostics.size; ++i) {
        String diagnostic = parser.diagnostics.data[i];
        printf("%" STRING_FMT "\n", STRING_PRINT(diagnostic));
      }
      printf("\x1b[0m");
    }
    MskExpressionSyntaxFree(syntax_tree);
    MskSyntaxParserFree(&parser);
    StringFree(&text);
  }
}
