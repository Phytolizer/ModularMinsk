#include <inttypes.h>
#include <minsk/code_analysis/syntax/lexer.h>
#include <minsk/code_analysis/syntax/token.h>
#include <minsk/runtime/object/object.h>
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

    MskLexer lexer = MskNewLexer(StringAsView(text));
    while (true) {
      MskSyntaxToken token = MskLexerLex(&lexer);
      if (token.kind == kMskSyntaxKindEndOfFileToken) {
        MskSyntaxTokenFree(&token);
        break;
      }
      printf("kind: %" STRING_VIEW_FMT "\n",
             STRING_VIEW_PRINT(MskSyntaxKindName(token.kind)));
      printf("text: %" STRING_FMT "\n", STRING_PRINT(token.text));
      printf("position: %" PRIu64 "\n", token.position);
      printf("value: ");
      MskRuntimeObjectPrint(&token.value);
      printf("\n");
      MskSyntaxTokenFree(&token);
    }
    StringFree(&text);
  }
}
