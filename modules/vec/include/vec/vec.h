#ifndef VEC_VEC_H_
#define VEC_VEC_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uint8_t** data;
  uint64_t* size;
  uint64_t* capacity;
  uint64_t sizeof_t;
} VecUnpacked;

#define VEC_TYPE(T)    \
  struct {             \
    T* data;           \
    uint64_t size;     \
    uint64_t capacity; \
  }

#define VEC_UNPACK(V)        \
  ((VecUnpacked){            \
      (uint8_t**)&(V)->data, \
      &(V)->size,            \
      &(V)->capacity,        \
      sizeof(*(V)->data),    \
  })

#define VEC_FREE(V)    \
  do {                 \
    free((V)->data);   \
    (V)->data = NULL;  \
    (V)->size = 0;     \
    (V)->capacity = 0; \
  } while (false)

#define VEC_PUSH(V, Value) \
  (VecExpand(VEC_UNPACK(V)) ? ((V)->data[(V)->size++] = (Value), true) : false)

#define VEC_POP(V) (V)->data[--(V)->size]

#define VEC_RESERVE(V, Amount) VecReserve(VEC_UNPACK(V), Amount)
#define VEC_RESIZE(V, Amount) VecResize(VEC_UNPACK(V), Amount)

#define VEC_APPEND(V, Data, Size) VecAppend(VEC_UNPACK(V), Data, Size)

bool VecExpand(VecUnpacked v);
bool VecReserve(VecUnpacked v, uint64_t amount);
bool VecResize(VecUnpacked v, uint64_t amount);
bool VecAppend(VecUnpacked v, const void* data, uint64_t size);

#endif  // VEC_VEC_H_
