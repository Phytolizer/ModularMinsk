#ifndef MINSK_CODE_ANALYSIS_SYNTAX_TOKEN_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_TOKEN_H_

#include <phyto/string/string.h>
#include <stdint.h>

#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/code_analysis/syntax/node.h"
#include "minsk/code_analysis/text/span.h"
#include "minsk/runtime/object.h"

typedef struct {
  MskSyntaxNode base;
  MskSyntaxKind kind;
  phyto_string_t text;
  uint64_t position;
  MskRuntimeObject value;
} MskSyntaxToken;

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_DECL(MskSyntaxTokens, MskSyntaxToken);

extern const MskSyntaxTokens_callbacks_t kMskSyntaxTokensCallbacks;

void MskSyntaxTokenFree(MskSyntaxToken* token);
MskSyntaxToken MskSyntaxTokenDuplicate(MskSyntaxToken token);
MskTextSpan MskSyntaxTokenGetSpan(MskSyntaxToken token);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_TOKEN_H_
