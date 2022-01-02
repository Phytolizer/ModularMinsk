#include "minsk/runtime/object.h"

#include <inttypes.h>
#include <stdlib.h>

static void FreeInteger(MskRuntimeObject* i);
static void FreeNull(MskRuntimeObject* n);

static void PrintInteger(MskRuntimeObject* i, FILE* fp);
static void PrintNull(MskRuntimeObject* n, FILE* fp);

StringView MskRuntimeObjectKindName(MskRuntimeObjectKind kind) {
  switch (kind) {
#define X(x)              \
  case kMskObjectKind##x: \
    return StringViewFromC(#x);
    MSK__OBJECT_KINDS
#undef X
    default:
      return StringViewFromC("<unknown>");
  }
}

MskRuntimeObject MskRuntimeObjectNewInteger(int64_t value) {
  return (MskRuntimeObject){
      .kind = kMskObjectKindInteger,
      .value = {.integer = value},
  };
}

void MskRuntimeObjectFree(MskRuntimeObject* object) {
  // you aren't allowed to pass NULL to this function, so i won't check.
  switch (object->kind) {
#define X(x)              \
  case kMskObjectKind##x: \
    Free##x(object);      \
    break;
    MSK__OBJECT_KINDS
#undef X
    default:
      break;
  }
  object->kind = kMskObjectKindNull;
}

void MskRuntimeObjectPrint(MskRuntimeObject* object, FILE* fp) {
  switch (object->kind) {
#define X(x)              \
  case kMskObjectKind##x: \
    Print##x(object, fp); \
    break;
    MSK__OBJECT_KINDS
#undef X
    default:
      break;
  }
}

MskRuntimeObject MskRuntimeObjectDuplicate(MskRuntimeObject* object) {
  // you aren't allowed to pass NULL to this function, so i won't check.
  switch (object->kind) {
    case kMskObjectKindInteger:
      return MskRuntimeObjectNewInteger(object->value.integer);
    case kMskObjectKindNull:
    default:
      return MSK_RUNTIME_OBJECT_NULL;
  }
}

void FreeInteger(MskRuntimeObject* i) {
  // integers don't need to be freed
  (void)i;
}

void FreeNull(MskRuntimeObject* n) {
  // null doesn't need to be freed
  (void)n;
}

void PrintInteger(MskRuntimeObject* i, FILE* fp) {
  fprintf(fp, "%" PRId64, i->value.integer);
}

void PrintNull(MskRuntimeObject* n, FILE* fp) {
  fprintf(fp, "<Null>");
}