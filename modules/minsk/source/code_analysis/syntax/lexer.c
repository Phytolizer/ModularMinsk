#include "minsk_private/code_analysis/syntax/lexer.h"

#include <stdint.h>

#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/code_analysis/syntax/node.h"
#include "minsk/runtime/object.h"
#include "string/string.h"

static char Cur(MskSyntaxLexer* lexer);

MskSyntaxLexer MskNewSyntaxLexer(StringView text) {
  return (MskSyntaxLexer){
      .text = text,
      .position = 0,
  };
}

MskSyntaxToken MskSyntaxLexerLex(MskSyntaxLexer* lexer) {
  MskSyntaxKind kind = kMskSyntaxKindBadToken;
  String text = {0};
  uint64_t position = lexer->position;
  MskRuntimeObject value = MSK_RUNTIME_OBJECT_NULL;

  switch (Cur(lexer)) {
    case '\0':
      kind = kMskSyntaxKindEndOfFileToken;
      break;
    case ' ':
    case '\t':
    case '\n':
    case '\r':
      while (Cur(lexer) == ' ' || Cur(lexer) == '\t' || Cur(lexer) == '\n' ||
             Cur(lexer) == '\r') {
        lexer->position++;
      }
      kind = kMskSyntaxKindWhitespaceToken;
      break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
      kind = kMskSyntaxKindNumberToken;
      while (Cur(lexer) >= '0' && Cur(lexer) <= '9') {
        lexer->position++;
      }
      // Create the text early. It's needed for StringViewToI64.
      text = StringFromView(
          StringViewSubstring(lexer->text, position, lexer->position));
      StringConversionResultI64 result = StringViewToI64(StringAsView(text));
      if (!result.success) {
        VEC_PUSH(&lexer->diagnostics,
                 StringFormat("The number '%" STRING_FMT
                              "' is too large for an int64_t.",
                              STRING_PRINT(text)));
      }
      value = MskRuntimeObjectNewInteger(result.value);
    } break;
    case '+':
      kind = kMskSyntaxKindPlusToken;
      lexer->position++;
      break;
    case '-':
      kind = kMskSyntaxKindMinusToken;
      lexer->position++;
      break;
    case '*':
      kind = kMskSyntaxKindStarToken;
      lexer->position++;
      break;
    case '/':
      kind = kMskSyntaxKindSlashToken;
      lexer->position++;
      break;
    case '(':
      kind = kMskSyntaxKindOpenParenthesisToken;
      lexer->position++;
      break;
    case ')':
      kind = kMskSyntaxKindCloseParenthesisToken;
      lexer->position++;
      break;
    default:
      VEC_PUSH(&lexer->diagnostics,
               StringFormat("Unexpected character '%c'", Cur(lexer)));
      lexer->position++;
      break;
  }

  // The text could have been created already.
  if (text.size == 0) {
    text = StringFromView(
        StringViewSubstring(lexer->text, position, lexer->position));
  }

  return (MskSyntaxToken){
      .base = {.kind = kMskSyntaxNodeKindToken},
      .kind = kind,
      .text = text,
      .position = position,
      .value = value,
  };
}

char Cur(MskSyntaxLexer* lexer) {
  if (lexer->position >= SPAN_SIZE(&lexer->text)) {
    return '\0';
  }
  return lexer->text.begin[lexer->position];
}
