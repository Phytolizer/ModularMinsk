#include <inttypes.h>
#include <minsk/code_analysis/syntax/lexer.h>
#include <minsk/code_analysis/syntax/parser.h>
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

    MskSyntaxParser parser = MskSyntaxParserNew(StringAsView(text));
    MskSyntaxParserFree(&parser);
    StringFree(&text);
  }
}
