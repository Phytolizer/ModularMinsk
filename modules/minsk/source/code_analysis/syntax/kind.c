#include "minsk/code_analysis/syntax/kind.h"

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_IMPL(MskSyntaxKinds, MskSyntaxKind);

static const char* const kMskSyntaxKindNames[] = {
#define X(x) [kMskSyntaxKind##x] = #x,
    MSK__SYNTAX_KINDS
#undef X
};

static void MskSyntaxKindPrint(MskSyntaxKind kind, FILE* stream) {
  fprintf(stream, "%s", kMskSyntaxKindNames[kind]);
}

const MskSyntaxKinds_callbacks_t kMskSyntaxKindsCallbacks = {
    .print_cb = MskSyntaxKindPrint,
};

MskSyntaxKinds_t MskSyntaxKindsGetAll(void) {
  static const MskSyntaxKind kAll[] = {
#define X(x) kMskSyntaxKind##x,
      MSK__SYNTAX_KINDS
#undef X
  };
  MskSyntaxKinds_t result = MskSyntaxKinds_init(&kMskSyntaxKindsCallbacks);
  MskSyntaxKinds_extend(&result,
                        MskSyntaxKinds_span_from_array(
                            kAll, sizeof(kAll) / sizeof(MskSyntaxKind)));
  return result;
}

phyto_string_span_t MskSyntaxKindName(MskSyntaxKind kind) {
  return phyto_string_span_from_c(kMskSyntaxKindNames[kind]);
}
