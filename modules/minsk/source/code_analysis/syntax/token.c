#include "minsk/code_analysis/syntax/token.h"

#include <stdlib.h>

#include "minsk/runtime/object/object.h"

void MskSyntaxTokenFree(MskSyntaxToken* token) {
  VEC_FREE(&token->text);
  MskRuntimeObjectFree(&token->value);
}
