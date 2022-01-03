#include <ansi_esc/ansi_esc.h>
#include <inttypes.h>
#include <minsk/code_analysis/compilation.h>
#include <minsk/code_analysis/syntax/expression.h>
#include <minsk/code_analysis/syntax/node.h>
#include <minsk/code_analysis/syntax/token.h>
#include <minsk/code_analysis/syntax/tree.h>
#include <minsk/runtime/object.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string/string.h>

#include "minsk/code_analysis/diagnostics.h"

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
    // move out the diagnostics because they aren't limited to the syntax tree
    MskDiagnostics diagnostics = syntax_tree.diagnostics;
    syntax_tree.diagnostics = (MskDiagnostics){0};
    if (show_tree) {
      MskSyntaxNodePrettyPrint(&syntax_tree.root->base, stdout, true);
    }
    bool show_diagnostics = false;
    if (diagnostics.size > 0) {
      show_diagnostics = true;
    } else {
      MskCompilation compilation = MskCompilationNew(syntax_tree);
      MskEvaluationResult result = MskCompilationEvaluate(&compilation);
      if (result.kind == kMskEvaluationResultFailure) {
        show_diagnostics = true;
        VEC_APPEND(&diagnostics, result.value.err.data, result.value.err.size);
        // shallow free -- the diagnostics vector owns the strings now
        VEC_FREE(&result.value.err);
      } else {
        MskRuntimeObjectPrint(&result.value.ok, stdout);
        printf("\n");
        MskRuntimeObjectFree(&result.value.ok);
      }
    }
    if (show_diagnostics) {
      printf("\n");
      printf(ANSI_ESC_DIM ANSI_ESC_FG_RED);
      for (size_t i = 0; i < diagnostics.size; ++i) {
        String diagnostic = diagnostics.data[i];
        printf("%" STRING_FMT "\n", STRING_PRINT(diagnostic));
      }
      printf(ANSI_ESC_RESET);
    }
    MskDiagnosticsFree(&diagnostics);
    MskSyntaxTreeFree(&syntax_tree);
    StringFree(&text);
  }
}
