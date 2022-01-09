#include "minsk/code_analysis/syntax/token.h"

#include <stdlib.h>

#include "minsk/runtime/object.h"
#include "phyto/string/string.h"

void MskSyntaxTokenFree(MskSyntaxToken* token) {
  phyto_string_free(&token->text);
  MskRuntimeObjectFree(&token->value);
}

void MskSyntaxTokensFree(MskSyntaxTokens* tokens) {
  for (size_t i = 0; i < VEC_SIZE(tokens); ++i) {
    MskSyntaxTokenFree(&tokens->data[i]);
  }
  VEC_FREE(tokens);
}

MskSyntaxToken MskSyntaxTokenDuplicate(MskSyntaxToken token) {
  MskSyntaxToken duplicate = token;
  duplicate.text = phyto_string_copy(token.text);
  duplicate.value = MskRuntimeObjectDuplicate(&token.value);
  return duplicate;
}

MskTextSpan MskSyntaxTokenGetSpan(MskSyntaxToken token) {
  return (MskTextSpan){
      .start = token.position,
      .length = token.text.size,
  };
}
