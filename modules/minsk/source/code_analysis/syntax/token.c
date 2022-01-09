#include "minsk/code_analysis/syntax/token.h"

#include <stdlib.h>

#include "minsk/runtime/object.h"
#include "phyto/string/string.h"

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_IMPL(MskSyntaxTokens, MskSyntaxToken);

const MskSyntaxTokens_callbacks_t kMskSyntaxTokensCallbacks = {
    .free_cb = MskSyntaxTokenFree,
    .copy_cb = MskSyntaxTokenDuplicate,
};

void MskSyntaxTokenFree(MskSyntaxToken* token) {
  phyto_string_free(&token->text);
  MskRuntimeObjectFree(&token->value);
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
