#include <ansi_esc/ansi_esc.h>
#include <inttypes.h>
#include <minsk/code_analysis/compilation.h>
#include <minsk/code_analysis/syntax/expression.h>
#include <minsk/code_analysis/syntax/node.h>
#include <minsk/code_analysis/syntax/token.h>
#include <minsk/code_analysis/syntax/tree.h>
#include <minsk/runtime/object.h>
#include <phyto/io/io.h>
#include <phyto/string/string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "minsk/code_analysis/symbol_table.h"
#include "minsk/code_analysis/text/diagnostic.h"
#include "minsk/code_analysis/text/diagnostic_bag.h"
#include "minsk/code_analysis/text/span.h"

int main(void) {
  bool show_tree = false;
  MskSymbolTable_t symbols = {0};
  while (true) {
    printf("minsk> ");
    fflush(stdout);
    phyto_string_t text = phyto_io_read_line(stdin);
    if (text.size == 0) {
      // EOF condition
      // Print a newline so the prompt isn't on the same line as the Bash
      // prompt.
      printf("\n");
      // Don't need to free an empty string.
      break;
    }
    if (phyto_string_span_equal(phyto_string_as_span(text),
                                phyto_string_span_from_c("#showTree\n"))) {
      show_tree = !show_tree;
      printf("showTree is now %s\n", show_tree ? "on" : "off");
      continue;
    }
    if (phyto_string_span_equal(phyto_string_as_span(text),
                                phyto_string_span_from_c("#cls\n"))) {
      printf(ANSI_ESC_KILL_SCREEN ANSI_ESC_CURSOR_RESET);
      continue;
    }
    if (phyto_string_span_equal(phyto_string_as_span(text),
                                phyto_string_span_from_c("#quit\n"))) {
      phyto_string_free(&text);
      break;
    }
    if (phyto_string_span_equal(phyto_string_as_span(text),
                                phyto_string_span_from_c("#help\n"))) {
      printf("#showTree - toggle showing the parse tree\n");
      printf("#cls      - clear the screen\n");
      printf("#help     - this message\n");
      printf("#quit     - exit the REPL\n");
      continue;
    }

    MskSyntaxTree syntax_tree = MskSyntaxTreeParse(phyto_string_as_span(text));
    // move out the diagnostics because they aren't limited to the syntax tree
    MskDiagnosticBag diagnostics = syntax_tree.diagnostics;
    syntax_tree.diagnostics = (MskDiagnosticBag)VEC_INIT_DEFAULT(MskDiagnostic);
    if (show_tree) {
      MskSyntaxNodePrettyPrint(&syntax_tree.root->base, stdout, true);
    }
    bool show_diagnostics = false;
    if (VEC_SIZE(&diagnostics) > 0) {
      show_diagnostics = true;
    } else {
      MskCompilation compilation = MskCompilationNew(syntax_tree);
      MskEvaluationResult result =
          MskCompilationEvaluate(&compilation, &symbols);
      if (result.kind == kMskEvaluationResultFailure) {
        show_diagnostics = true;
        VEC_APPEND(&diagnostics, result.value.err.data,
                   VEC_SIZE(&result.value.err));
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
      for (size_t i = 0; i < VEC_SIZE(&diagnostics); ++i) {
        phyto_string_span_t prefix = phyto_string_span_subspan(
            phyto_string_as_span(text), 0, diagnostics.data[i].span.start);
        phyto_string_span_t error = phyto_string_span_subspan(
            phyto_string_as_span(text), diagnostics.data[i].span.start,
            MskTextSpanEnd(diagnostics.data[i].span));
        phyto_string_span_t suffix = phyto_string_span_subspan(
            phyto_string_as_span(text),
            MskTextSpanEnd(diagnostics.data[i].span), text.size);
        printf("%" PHYTO_STRING_FORMAT ANSI_ESC_FG_RED
               "%" PHYTO_STRING_FORMAT ANSI_ESC_RESET "%" PHYTO_STRING_FORMAT
               "\n",
               PHYTO_STRING_VIEW_PRINTF_ARGS(prefix),
               PHYTO_STRING_VIEW_PRINTF_ARGS(error),
               PHYTO_STRING_VIEW_PRINTF_ARGS(suffix));
        MskDiagnostic diagnostic = diagnostics.data[i];
        printf(ANSI_ESC_FG_RED);
        printf("%" PHYTO_STRING_FORMAT "\n",
               PHYTO_STRING_PRINTF_ARGS(diagnostic.message));
        printf(ANSI_ESC_RESET);
      }
    }
    MskDiagnosticBagFree(&diagnostics);
    MskSyntaxTreeFree(&syntax_tree);
    phyto_string_free(&text);
  }
  MskSymbolTable_free(&symbols);
}
