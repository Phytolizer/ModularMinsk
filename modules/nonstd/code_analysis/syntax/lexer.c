#include "minsk/code_analysis/syntax/lexer.h"

MskLexer MskNewLexer(StringView text) {
  return (MskLexer){
      .text = text,
      .position = 0,
      .diagnostics = {0},
  };
}
