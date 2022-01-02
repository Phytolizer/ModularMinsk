#ifndef HASH_HASH_H_
#define HASH_HASH_H_

#include <span/span.h>
#include <stdbool.h>
#include <stdint.h>
#include <vec/vec.h>

typedef SPAN_TYPE(uint8_t) HashKeySpan;
typedef VEC_TYPE(uint8_t) HashKeyVec;

typedef struct {
  HashKeyVec vec;
  uint64_t hash;
} HashKey;

typedef struct {
  HashKeySpan span;
  uint64_t hash;
} HashKeyView;

typedef struct {
  HashKey** keys;
  uint8_t** values;
  uint64_t* size;
  uint64_t* capacity;
  uint64_t sizeof_value;
} HashUnpacked;

#define HASH_UNPACK(Hash)                      \
  ((HashUnpacked){                             \
      .keys = &(Hash)->keys,                   \
      .values = (uint8_t**)&(Hash)->values,    \
      .size = &(Hash)->size,                   \
      .capacity = &(Hash)->capacity,           \
      .sizeof_value = sizeof(*(Hash)->values), \
  })

#define HASH_TYPE(T)   \
  struct {             \
    HashKey* keys;     \
    T* values;         \
    uint64_t size;     \
    uint64_t capacity; \
  }

typedef enum {
  kHashAddSuccess,
  kHashAddFailure,
  kHashAddReplace,
} HashAddResult;

#define HASH_ADD(Hash, Key, Value) \
  HashAdd(HASH_UNPACK(Hash), HashCreateKey(Key), (const uint8_t*)&(Value))
#define HASH_FREE(Hash) HashFree(HASH_UNPACK(Hash))
#define HASH_GET(Hash, Key, OutValue) \
  HashGet(HASH_UNPACK(Hash), HashCreateKey(Key), (uint8_t*)(OutValue))

HashAddResult HashAdd(HashUnpacked hash, HashKeyView key, const uint8_t* value);
HashKeyView HashCreateKey(HashKeySpan key);
void HashFree(HashUnpacked hash);
bool HashGet(HashUnpacked hash, HashKeyView key, uint8_t* out_value);

#endif  // HASH_HASH_H_
