#ifndef MINSK_CODE_ANALYSIS_SYNTAX_TOKEN_H_
#define MINSK_CODE_ANALYSIS_SYNTAX_TOKEN_H_

#include <stdint.h>
#include <string/string.h>

#include "minsk/code_analysis/syntax/kind.h"
#include "minsk/code_analysis/syntax/node.h"
#include "minsk/code_analysis/text/span.h"
#include "minsk/runtime/object.h"

typedef struct {
  MskSyntaxNode base;
  MskSyntaxKind kind;
  String text;
  uint64_t position;
  MskRuntimeObject value;
} MskSyntaxToken;

void MskSyntaxTokenFree(MskSyntaxToken* token);
MskSyntaxToken MskSyntaxTokenDuplicate(MskSyntaxToken token);
MskTextSpan MskSyntaxTokenGetSpan(MskSyntaxToken* token);

#endif  // MINSK_CODE_ANALYSIS_SYNTAX_TOKEN_H_
