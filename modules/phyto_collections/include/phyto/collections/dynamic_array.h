#ifndef PHYTO_COLLECTIONS_DYNAMIC_ARRAY_H_
#define PHYTO_COLLECTIONS_DYNAMIC_ARRAY_H_

#include <nonstd/qsort.h>
#include <phyto/span/span.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "phyto/collections/callbacks.h"

#define PHYTO_COLLECTIONS_DYNAMIC_ARRAY_DECL(Name, DataType)                 \
  typedef enum {                                                             \
    Name##_error_flag_ok,                                                    \
    Name##_error_flag_out_of_memory,                                         \
    Name##_error_flag_out_of_bounds,                                         \
    Name##_error_flag_not_found,                                             \
    Name##_error_flag_invalid_argument,                                      \
  } Name##_error_flag_t;                                                     \
                                                                             \
  const char* Name##_explain_error(Name##_error_flag_t error_flag);          \
                                                                             \
  typedef struct {                                                           \
    PHYTO_COLLECTIONS_FREE_CB(DataType);                                     \
    PHYTO_COLLECTIONS_COPY_CB(DataType);                                     \
    PHYTO_COLLECTIONS_COMPARE_CB(DataType);                                  \
    PHYTO_COLLECTIONS_PRINT_CB(DataType);                                    \
  } Name##_callbacks_t;                                                      \
  typedef PHYTO_SPAN_TYPE(DataType) Name##_span_t;                           \
                                                                             \
  Name##_span_t Name##_span_new(const DataType* begin, const DataType* end); \
  Name##_span_t Name##_span_empty(void);                                     \
  Name##_span_t Name##_span_from_array(const DataType* array, size_t size);  \
  Name##_span_t Name##_span_subspan(Name##_span_t span, size_t begin,        \
                                    size_t end);                             \
                                                                             \
  typedef struct {                                                           \
    DataType* data;                                                          \
    size_t size;                                                             \
    size_t capacity;                                                         \
    const Name##_callbacks_t* callbacks;                                     \
    Name##_error_flag_t error_flag;                                          \
  } Name##_t;                                                                \
                                                                             \
  Name##_t Name##_init(const Name##_callbacks_t* callbacks);                 \
  Name##_t Name##_init_from_span(const Name##_callbacks_t* callbacks,        \
                                 Name##_span_t span);                        \
  void Name##_free(Name##_t* self);                                          \
  bool Name##_reserve(Name##_t* self, size_t new_capacity);                  \
  bool Name##_resize(Name##_t* self, size_t new_size);                       \
  bool Name##_insert(Name##_t* self, size_t index, DataType value);          \
  bool Name##_append(Name##_t* self, DataType value);                        \
  bool Name##_extend(Name##_t* self, Name##_span_t span);                    \
  bool Name##_remove(Name##_t* self, DataType value);                        \
  bool Name##_remove_at(Name##_t* self, size_t index);                       \
  size_t Name##_index(Name##_t* self, DataType value);                       \
  bool Name##_contains(Name##_t self, DataType value);                       \
  size_t Name##_count(Name##_t self, DataType value);                        \
  void Name##_clear(Name##_t* self);                                         \
  void Name##_sort(Name##_t* self);                                          \
  void Name##_reverse(Name##_t* self);                                       \
  Name##_t Name##_copy(Name##_t self);                                       \
  void Name##_string(Name##_t self,                                          \
                     void (*cb)(const char* data, size_t len, void* state),  \
                     void* state);                                           \
  void Name##_print(Name##_t self, FILE* fp, const char* sep);               \
  bool Name##_equals(Name##_t self, Name##_t other);                         \
  Name##_span_t Name##_range(Name##_t* self, size_t begin, size_t end);      \
  Name##_span_t Name##_as_span(Name##_t self);

#define PHYTO_COLLECTIONS_DYNAMIC_ARRAY_IMPL(Name, DataType)                  \
  const char* Name##_explain_error(Name##_error_flag_t error_flag) {          \
    switch (error_flag) {                                                     \
      case Name##_error_flag_ok:                                              \
        return "ok";                                                          \
      case Name##_error_flag_out_of_memory:                                   \
        return "out of memory";                                               \
      case Name##_error_flag_out_of_bounds:                                   \
        return "out of bounds";                                               \
      case Name##_error_flag_not_found:                                       \
        return "element not found";                                           \
      case Name##_error_flag_invalid_argument:                                \
        return "invalid argument";                                            \
      default:                                                                \
        return "unknown error";                                               \
    }                                                                         \
  }                                                                           \
  Name##_span_t Name##_span_new(const DataType* begin, const DataType* end) { \
    return (Name##_span_t)PHYTO_SPAN_NEW(begin, end);                         \
  }                                                                           \
  Name##_span_t Name##_span_empty(void) {                                     \
    return (Name##_span_t)PHYTO_SPAN_EMPTY;                                   \
  }                                                                           \
  Name##_span_t Name##_span_from_array(const DataType* array, size_t size) {  \
    return (Name##_span_t)PHYTO_SPAN_NEW(array, array + size);                \
  }                                                                           \
  Name##_span_t Name##_span_subspan(Name##_span_t span, size_t begin,         \
                                    size_t end) {                             \
    if (begin >= end || end > span.size) {                                    \
      return (Name##_span_t)PHYTO_SPAN_EMPTY;                                 \
    }                                                                         \
    return (Name##_span_t)PHYTO_SPAN_NEW(span.begin + begin,                  \
                                         span.begin + end);                   \
  }                                                                           \
  Name##_t Name##_init(const Name##_callbacks_t* callbacks) {                 \
    Name##_t self = {                                                         \
        .data = NULL,                                                         \
        .size = 0,                                                            \
        .capacity = 0,                                                        \
        .callbacks = callbacks,                                               \
        .error_flag = Name##_error_flag_ok,                                   \
    };                                                                        \
    return self;                                                              \
  }                                                                           \
  Name##_t Name##_init_from_span(const Name##_callbacks_t* callbacks,         \
                                 Name##_span_t span) {                        \
    Name##_t self = Name##_init(callbacks);                                   \
    if (!Name##_extend(&self, span)) {                                        \
      Name##_free(&self);                                                     \
      return (Name##_t){.error_flag = Name##_error_flag_out_of_memory};       \
    }                                                                         \
    return self;                                                              \
  }                                                                           \
  void Name##_free(Name##_t* self) {                                          \
    if (!self->callbacks) {                                                   \
      return;                                                                 \
    }                                                                         \
    if (self->callbacks->free_cb) {                                           \
      for (size_t i = 0; i < self->size; ++i) {                               \
        self->callbacks->free_cb(&self->data[i]);                             \
      }                                                                       \
    }                                                                         \
    free(self->data);                                                         \
    self->data = NULL;                                                        \
    self->size = 0;                                                           \
    self->capacity = 0;                                                       \
    self->callbacks = NULL;                                                   \
    self->error_flag = Name##_error_flag_ok;                                  \
  }                                                                           \
  bool Name##_reserve(Name##_t* self, size_t new_capacity) {                  \
    if (new_capacity <= self->capacity) {                                     \
      return true;                                                            \
    }                                                                         \
    size_t old_capacity = self->capacity;                                     \
    while (new_capacity > self->capacity) {                                   \
      self->capacity = self->capacity * 2 + 1;                                \
    }                                                                         \
    DataType* new_data =                                                      \
        realloc(self->data, self->capacity * sizeof(DataType));               \
    if (!new_data) {                                                          \
      self->error_flag = Name##_error_flag_out_of_memory;                     \
      return false;                                                           \
    }                                                                         \
    memset(new_data + old_capacity, 0,                                        \
           (self->capacity - old_capacity) * sizeof(DataType));               \
    self->data = new_data;                                                    \
    return true;                                                              \
  }                                                                           \
  bool Name##_resize(Name##_t* self, size_t new_size) {                       \
    if (new_size < self->size) {                                              \
      self->error_flag = Name##_error_flag_invalid_argument;                  \
      return false;                                                           \
    }                                                                         \
    if (new_size <= self->capacity) {                                         \
      self->size = new_size;                                                  \
      return true;                                                            \
    }                                                                         \
    if (!Name##_reserve(self, new_size)) {                                    \
      return false;                                                           \
    }                                                                         \
    self->size = new_size;                                                    \
    return true;                                                              \
  }                                                                           \
  bool Name##_insert(Name##_t* self, size_t index, DataType value) {          \
    if (index > self->size) {                                                 \
      self->error_flag = Name##_error_flag_invalid_argument;                  \
      return false;                                                           \
    }                                                                         \
    if (!Name##_reserve(self, self->size + 1)) {                              \
      return false;                                                           \
    }                                                                         \
    memmove(self->data + index + 1, self->data + index,                       \
            (self->size - index) * sizeof(DataType));                         \
    self->data[index] = value;                                                \
    ++self->size;                                                             \
    return true;                                                              \
  }                                                                           \
  bool Name##_append(Name##_t* self, DataType value) {                        \
    return Name##_insert(self, self->size, value);                            \
  }                                                                           \
  bool Name##_extend(Name##_t* self, Name##_span_t span) {                    \
    if (span.size == 0) {                                                     \
      return true;                                                            \
    }                                                                         \
    if (!Name##_resize(self, self->size + span.size)) {                       \
      return false;                                                           \
    }                                                                         \
    if (self->callbacks->copy_cb) {                                           \
      for (size_t i = 0; i < span.size; ++i) {                                \
        self->data[self->size - span.size + i] =                              \
            self->callbacks->copy_cb(span.begin[i]);                          \
      }                                                                       \
    } else {                                                                  \
      memcpy(self->data + self->size - span.size, span.begin,                 \
             span.size * sizeof(DataType));                                   \
    }                                                                         \
    return true;                                                              \
  }                                                                           \
  bool Name##_remove(Name##_t* self, DataType value) {                        \
    for (size_t i = 0; i < self->size; ++i) {                                 \
      if (self->callbacks->compare_cb(self->data[i], value) == 0) {           \
        return Name##_remove_at(self, i);                                     \
      }                                                                       \
    }                                                                         \
    self->error_flag = Name##_error_flag_not_found;                           \
    return false;                                                             \
  }                                                                           \
  bool Name##_remove_at(Name##_t* self, size_t index) {                       \
    if (index >= self->size) {                                                \
      self->error_flag = Name##_error_flag_invalid_argument;                  \
      return false;                                                           \
    }                                                                         \
    if (self->callbacks->free_cb) {                                           \
      self->callbacks->free_cb(&self->data[index]);                           \
    }                                                                         \
    memmove(self->data + index, self->data + index + 1,                       \
            (self->size - index - 1) * sizeof(DataType));                     \
    --self->size;                                                             \
    return true;                                                              \
  }                                                                           \
  size_t Name##_index(Name##_t* self, DataType value) {                       \
    for (size_t i = 0; i < self->size; ++i) {                                 \
      if (self->callbacks->compare_cb(self->data[i], value) == 0) {           \
        return i;                                                             \
      }                                                                       \
    }                                                                         \
    self->error_flag = Name##_error_flag_not_found;                           \
    return self->size;                                                        \
  }                                                                           \
  bool Name##_contains(Name##_t self, DataType value) {                       \
    return Name##_count(self, value) > 0;                                     \
  }                                                                           \
  size_t Name##_count(Name##_t self, DataType value) {                        \
    size_t count = 0;                                                         \
    for (size_t i = 0; i < self.size; ++i) {                                  \
      if (self.callbacks->compare_cb(self.data[i], value) == 0) {             \
        ++count;                                                              \
      }                                                                       \
    }                                                                         \
    return count;                                                             \
  }                                                                           \
  void Name##_clear(Name##_t* self) {                                         \
    if (self->callbacks->free_cb) {                                           \
      for (size_t i = 0; i < self->size; ++i) {                               \
        self->callbacks->free_cb(&self->data[i]);                             \
      }                                                                       \
    }                                                                         \
    self->size = 0;                                                           \
  }                                                                           \
  static int Name##_compare_values(const void* a, const void* b,              \
                                   void* state) {                             \
    Name##_t* self = state;                                                   \
    DataType ac = *(DataType*)a;                                              \
    DataType bc = *(DataType*)b;                                              \
    return self->callbacks->compare_cb(ac, bc);                               \
  }                                                                           \
  void Name##_sort(Name##_t* self) {                                          \
    if (self->size > 1) {                                                     \
      nonstd_qsort_r(self->data, self->size, sizeof(DataType),                \
                     Name##_compare_values, self);                            \
    }                                                                         \
  }                                                                           \
  void Name##_reverse(Name##_t* self) {                                       \
    if (self->size > 1) {                                                     \
      for (size_t i = 0; i < self->size / 2; ++i) {                           \
        DataType tmp = self->data[i];                                         \
        self->data[i] = self->data[self->size - i - 1];                       \
        self->data[self->size - i - 1] = tmp;                                 \
      }                                                                       \
    }                                                                         \
  }                                                                           \
  Name##_t Name##_copy(Name##_t self) {                                       \
    return Name##_init_from_span(self.callbacks, Name##_as_span(self));       \
  }                                                                           \
  void Name##_string(Name##_t self,                                           \
                     void (*cb)(const char* data, size_t len, void* state),   \
                     void* state) {                                           \
    cb(#Name, sizeof(#Name) - 1, state);                                      \
    cb("<size:", 6, state);                                                   \
    char buf[32];                                                             \
    int len = snprintf(buf, sizeof(buf), "%zu", self.size);                   \
    cb(buf, len, state);                                                      \
    cb(",capacity:", 10, state);                                              \
    len = snprintf(buf, sizeof(buf), "%zu", self.capacity);                   \
    cb(buf, len, state);                                                      \
    cb(",data:", 6, state);                                                   \
    len = snprintf(buf, sizeof(buf), "%p", self.data);                        \
    cb(buf, len, state);                                                      \
    cb(">", 1, state);                                                        \
  }                                                                           \
  void Name##_print(Name##_t self, FILE* fp, const char* sep) {               \
    for (size_t i = 0; i < self.size; ++i) {                                  \
      self.callbacks->print_cb(self.data[i], fp);                             \
      if (i < self.size - 1) {                                                \
        fprintf(fp, "%s", sep);                                               \
      }                                                                       \
    }                                                                         \
  }                                                                           \
  bool Name##_equals(Name##_t self, Name##_t other) {                         \
    if (self.size != other.size) {                                            \
      return false;                                                           \
    }                                                                         \
    for (size_t i = 0; i < self.size; ++i) {                                  \
      if (self.callbacks->compare_cb(self.data[i], other.data[i]) != 0) {     \
        return false;                                                         \
      }                                                                       \
    }                                                                         \
    return true;                                                              \
  }                                                                           \
  Name##_span_t Name##_range(Name##_t* self, size_t begin, size_t end) {      \
    if (begin > end || begin > self->size) {                                  \
      self->error_flag = Name##_error_flag_out_of_bounds;                     \
      return (Name##_span_t){0};                                              \
    }                                                                         \
    if (end > self->size) {                                                   \
      end = self->size;                                                       \
    }                                                                         \
    return Name##_span_new((const DataType*)(self->data + begin),             \
                           (const DataType*)(self->data + end));              \
  }                                                                           \
  Name##_span_t Name##_as_span(Name##_t self) {                               \
    return Name##_span_from_array((const DataType*)self.data, self.size);     \
  }

#endif  // PHYTO_COLLECTIONS_DYNAMIC_ARRAY_H_
