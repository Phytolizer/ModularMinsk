#include "minsk/code_analysis/syntax/lexer.h"

#include <stdint.h>

#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/runtime/object/object.h"
#include "string/string.h"

static char Cur(MskLexer* lexer);

MskLexer MskNewLexer(StringView text) {
  return (MskLexer){
      .text = text,
      .position = 0,
  };
}

MskSyntaxToken MskLexerLex(MskLexer* lexer) {
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
      text = StringFromSpan(
          StringViewSubstring(lexer->text, position, lexer->position));
      StringConversionResultI64 result = StringViewToI64(StringAsView(text));
      if (!result.success) {
        // TODO: Report error.
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
      lexer->position++;
      break;
  }

  // The text could have been created already.
  if (text.size == 0) {
    text = StringFromSpan(
        StringViewSubstring(lexer->text, position, lexer->position));
  }

  return (MskSyntaxToken){
      .kind = kind,
      .text = text,
      .position = position,
      .value = value,
  };
}

char Cur(MskLexer* lexer) {
  if (lexer->position >= SPAN_SIZE(&lexer->text)) {
    return '\0';
  }
  return lexer->text.begin[lexer->position];
}
