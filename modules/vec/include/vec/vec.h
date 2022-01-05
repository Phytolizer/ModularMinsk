#ifndef VEC_VEC_H_
#define VEC_VEC_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum {
  kVecShrinkNever,
  kVecShrinkWhenEmpty,
  kVecShrinkAtPercentage,
} VecShrinkMode;

typedef enum {
  kVecOpPush,
  kVecOpPop,
} VecOp;

typedef struct {
  uint8_t* data;
  uint64_t size;
  uint64_t capacity;
  uint64_t sizeof_t;
  uint64_t min_capacity;
  VecShrinkMode shrink_mode;
  uint8_t shrink_percentage;
} Vec;

#define VEC_TYPE(T) \
  union {           \
    Vec base;       \
    T* data;        \
  }

#define VEC_INIT_SHRINKABLE(SizeofType, MinCapacity, ShrinkMode, \
                            ShrinkPercentage)                    \
  {                                                              \
    .base = {                                                    \
      .data = NULL,                                              \
      .size = 0,                                                 \
      .capacity = 0,                                             \
      .sizeof_t = SizeofType,                                    \
      .min_capacity = MinCapacity,                               \
      .shrink_mode = ShrinkMode,                                 \
      .shrink_percentage = ShrinkPercentage                      \
    }                                                            \
  }

#define VEC_INIT(SizeofType, MinCapacity) \
  VEC_INIT_SHRINKABLE(SizeofType, MinCapacity, kVecShrinkNever, 0)

#define VEC_INIT_DEFAULT(T) VEC_INIT(sizeof(T), 8)

#define VEC_PUSH(V, Value)   \
  ({                         \
    typeof(Value) v = Value; \
    VecPush(&(V)->base, &v); \
  })
#define VEC_APPEND(V, Values, Length) VecAppend(&(V)->base, Values, Length)
#define VEC_POP(V) VecPop(&(V)->base)
#define VEC_POP_OUT(V, Value) VecPopOut(&(V)->base, Value)

#define VEC_FREE(V) VecFree(&(V)->base)

#define VEC_SIZE(V) ((V)->base.size)
#define VEC_RESIZE(V, Size)                 \
  ({                                        \
    bool ret = VecResize(&(V)->base, Size); \
    if (ret) {                              \
      (V)->base.size = Size;                \
    }                                       \
    ret;                                    \
  })

bool VecResize(Vec* v, uint64_t amount);
bool VecPush(Vec* v, const void* data);
bool VecAppend(Vec* v, const void* data, uint64_t size);
void* VecPop(Vec* v);
bool VecPopOut(Vec* v, void* out);
void VecFree(Vec* v);

#endif  // VEC_VEC_H_
