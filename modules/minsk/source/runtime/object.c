#include "minsk/runtime/object.h"

#include <inttypes.h>
#include <stdlib.h>

static void FreeInteger(MskRuntimeObject* i);
static void FreeNull(MskRuntimeObject* n);
static void FreeBoolean(MskRuntimeObject* b);

static void PrintInteger(MskRuntimeObject* i, FILE* fp);
static void PrintNull(MskRuntimeObject* n, FILE* fp);
static void PrintBoolean(MskRuntimeObject* b, FILE* fp);

static const char* const kMskObjectKindNames[] = {
    [kMskObjectKindInvalid] = "Invalid",
#define X(x) [kMskObjectKind##x] = #x,
    MSK__OBJECT_KINDS
#undef X
};

StringView MskRuntimeObjectKindName(MskRuntimeObjectKind kind) {
  return StringViewFromC(kMskObjectKindNames[kind]);
}

MskRuntimeObject MskRuntimeObjectNewInteger(int64_t value) {
  return (MskRuntimeObject){
      .kind = kMskObjectKindInteger,
      .value = {.integer = value},
  };
}

MskRuntimeObject MskRuntimeObjectNewBoolean(bool value) {
  return (MskRuntimeObject){
      .kind = kMskObjectKindBoolean,
      .value = {.boolean = value},
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

bool MskRuntimeObjectEquals(MskRuntimeObject* a, MskRuntimeObject* b) {
  if (a->kind != b->kind) {
    return false;
  }
  switch (a->kind) {
    case kMskObjectKindInteger:
      return a->value.integer == b->value.integer;
    case kMskObjectKindBoolean:
      return a->value.boolean == b->value.boolean;
    case kMskObjectKindNull:
    default:
      return true;
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

void FreeBoolean(MskRuntimeObject* b) {
  // booleans don't need to be freed
  (void)b;
}

void PrintInteger(MskRuntimeObject* i, FILE* fp) {
  fprintf(fp, "%" PRId64, i->value.integer);
}

void PrintNull(MskRuntimeObject* n, FILE* fp) {
  fprintf(fp, "<Null>");
}

void PrintBoolean(MskRuntimeObject* b, FILE* fp) {
  fprintf(fp, "%s", b->value.boolean ? "true" : "false");
}
