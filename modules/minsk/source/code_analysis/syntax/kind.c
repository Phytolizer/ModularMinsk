#include "minsk/code_analysis/syntax/kind.h"

static const char* const kMskSyntaxKindNames[] = {
#define X(x) [kMskSyntaxKind##x] = #x,
    MSK__SYNTAX_KINDS
#undef X
};

MskSyntaxKinds MskSyntaxKindsGetAll(void) {
  static const MskSyntaxKind kAll[] = {
#define X(x) kMskSyntaxKind##x,
      MSK__SYNTAX_KINDS
#undef X
  };
  MskSyntaxKinds result = VEC_INIT_DEFAULT(MskSyntaxKind);
  VEC_APPEND(&result, kAll, sizeof(kAll) / sizeof(MskSyntaxKind));
  return result;
}

phyto_string_span_t MskSyntaxKindName(MskSyntaxKind kind) {
  return phyto_string_span_from_c(kMskSyntaxKindNames[kind]);
}
