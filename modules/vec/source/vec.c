#include "vec/vec.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

bool VecResize(Vec* v, uint64_t amount) {
  while (v->capacity < amount) {
    if (v->capacity == 0) {
      assert(v->min_capacity != 0 &&
             "VecResize: min_capacity is 0, did you memset?");
      v->capacity = v->min_capacity;
    } else {
      v->capacity *= 2;
    }
  }
  uint8_t* new_data = realloc(v->data, v->capacity * v->sizeof_t);
  if (new_data == NULL) {
    return false;
  }
  v->data = new_data;
  return true;
}

bool VecPush(Vec* v, const void* data) {
  if (!VecResize(v, v->size + 1)) {
    return false;
  }
  memcpy(v->data + v->size * v->sizeof_t, data, v->sizeof_t);
  v->size++;
  return true;
}

bool VecAppend(Vec* v, const void* data, uint64_t size) {
  if (!VecResize(v, v->size + size)) {
    return false;
  }
  memcpy(v->data + v->size * v->sizeof_t, data, size * v->sizeof_t);
  v->size += size;
  return true;
}

void* VecPop(Vec* v) {
  if (v->size == 0) {
    return NULL;
  }
  v->size--;
  return v->data + v->size * v->sizeof_t;
}

bool VecPopOut(Vec* v, void* out) {
  if (v->size == 0) {
    return false;
  }
  v->size--;
  memcpy(out, v->data + v->size * v->sizeof_t, v->sizeof_t);
  return true;
}

void VecFree(Vec* v) {
  free(v->data);
  v->data = NULL;
  v->size = 0;
  v->capacity = 0;
}