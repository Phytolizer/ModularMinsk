#include <ansi_esc/ansi_esc.h>
#include <inttypes.h>
#include <minsk/code_analysis/evaluator.h>
#include <minsk/code_analysis/syntax/expression.h>
#include <minsk/code_analysis/syntax/node.h>
#include <minsk/code_analysis/syntax/token.h>
#include <minsk/code_analysis/syntax/tree.h>
#include <minsk/runtime/object.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string/string.h>

int main(void) {
  bool show_tree = false;
  while (true) {
    printf("minsk> ");
    fflush(stdout);
    String text = StringGetLine(stdin);
    if (text.size == 0) {
      // EOF condition
      // Print a newline so the prompt isn't on the same line as the Bash
      // prompt.
      printf("\n");
      // Don't need to free an empty string.
      break;
    }
    if (StringEqualC(text, "#showTree\n")) {
      show_tree = !show_tree;
      printf("showTree is now %s\n", show_tree ? "on" : "off");
      continue;
    }
    if (StringEqualC(text, "#cls\n")) {
      printf(ANSI_ESC_KILL_SCREEN ANSI_ESC_CURSOR_RESET);
      continue;
    }
    if (StringEqualC(text, "#quit\n")) {
      StringFree(&text);
      break;
    }
    if (StringEqualC(text, "#help\n")) {
      printf("#showTree - toggle showing the parse tree\n");
      printf("#cls      - clear the screen\n");
      printf("#help     - this message\n");
      printf("#quit     - exit the REPL\n");
      continue;
    }

    MskSyntaxTree syntax_tree = MskSyntaxTreeParse(StringAsView(text));
    if (show_tree) {
      MskSyntaxNodePrettyPrint(&syntax_tree.root->base, stdout, true);
    }
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
