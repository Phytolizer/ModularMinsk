#include "minsk/code_analysis/syntax/lexer.h"

MskSyntaxLexer MskNewSyntaxLexer(StringView text) {
  return (MskSyntaxLexer){
      .text = text,
      .position = 0,
      .diagnostics = {0},
  };
}
