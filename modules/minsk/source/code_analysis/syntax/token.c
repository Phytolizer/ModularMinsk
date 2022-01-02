#include "minsk/code_analysis/syntax/token.h"

#include <stdlib.h>

#include "minsk/runtime/object.h"

void MskSyntaxTokenFree(MskSyntaxToken* token) {
  StringFree(&token->text);
  MskRuntimeObjectFree(&token->value);
}

MskSyntaxToken MskSyntaxTokenDuplicate(MskSyntaxToken token) {
  MskSyntaxToken duplicate = token;
  duplicate.text = StringDuplicate(token.text);
  duplicate.value = MskRuntimeObjectDuplicate(&token.value);
  return duplicate;
}
