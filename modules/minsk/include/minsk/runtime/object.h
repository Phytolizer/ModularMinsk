#ifndef MINSK_RUNTIME_OBJECT_H_
#define MINSK_RUNTIME_OBJECT_H_

#include <annotate/annotate.h>
#include <stdint.h>
#include <stdio.h>
#include <string/string.h>

#define MSK__OBJECT_KINDS \
  X(Null)                 \
  X(Integer)

typedef enum {
  kMskObjectKindInvalid,
#define X(x) kMskObjectKind##x,
  MSK__OBJECT_KINDS
#undef X
} MskRuntimeObjectKind;

StringView MskRuntimeObjectKindName(MskRuntimeObjectKind kind);

typedef struct {
  MskRuntimeObjectKind kind;
  union {
    int64_t integer;
  } value;
} MskRuntimeObject;

#define MSK_RUNTIME_OBJECT_NULL ((MskRuntimeObject){.kind = kMskObjectKindNull})
MskRuntimeObject MskRuntimeObjectNewInteger(int64_t value);
void MskRuntimeObjectFree(MskRuntimeObject* object) ANNOTATE_NONNULL(1);
void MskRuntimeObjectPrint(MskRuntimeObject* object, FILE* fp);
MskRuntimeObject MskRuntimeObjectDuplicate(MskRuntimeObject* object);

#endif  // MINSK_RUNTIME_OBJECT_H_
