#ifndef PHYTO_STRING_STRING_H_
#define PHYTO_STRING_STRING_H_

#include <phyto/collections/dynamic_array.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_DECL(phyto_string, char)
PHYTO_COLLECTIONS_DYNAMIC_ARRAY_DECL(phyto_string_vec, phyto_string_t)

#define PHYTO_STRING_FORMAT ".*s"
#define PHYTO_STRING_PRINTF_ARGS(Str) (int)(Str).size, (Str).data
#define PHYTO_STRING_VIEW_PRINTF_ARGS(Sview) (int)(Sview).size, (Sview).begin

typedef struct {
  bool success;
  int64_t value;
} phyto_string_conversion_result_i64_t;

extern const phyto_string_vec_callbacks_t phyto_string_vec_callbacks;

phyto_string_t phyto_string_new(void);
phyto_string_t phyto_string_own(phyto_string_span_t span);
phyto_string_t phyto_string_from_c(const char* str);
phyto_string_span_t phyto_string_span_from_c(const char* str);
bool phyto_string_span_equal(phyto_string_span_t a, phyto_string_span_t b);
phyto_string_t phyto_string_from_sprintf(const char* format, ...);
phyto_string_t phyto_string_copy(phyto_string_t str);
int32_t phyto_string_compare(phyto_string_span_t lhs, phyto_string_span_t rhs);
bool phyto_string_reserve(phyto_string_t* string, size_t capacity);
bool phyto_string_append_fill(phyto_string_t* string, size_t count, char fill);
void phyto_string_print_nosep(phyto_string_t string, FILE* stream);

bool phyto_string_append_c(phyto_string_t* string, const char* cstr);
void phyto_string_span_print_to(phyto_string_span_t span, FILE* stream);
phyto_string_conversion_result_i64_t phyto_string_to_i64(
    phyto_string_span_t span);

phyto_string_t phyto_string_capitalize(phyto_string_span_t span);
phyto_string_t phyto_string_center(phyto_string_span_t span,
                                   size_t width,
                                   char fill);
size_t phyto_string_count_sub(phyto_string_span_t span,
                              phyto_string_span_t sub);
size_t phyto_string_count_sub_in_range(phyto_string_span_t span,
                                       phyto_string_span_t sub,
                                       size_t start,
                                       size_t end);
bool phyto_string_ends_with(phyto_string_span_t view, phyto_string_span_t sub);
bool phyto_string_starts_with(phyto_string_span_t view,
                              phyto_string_span_t sub);
phyto_string_t phyto_string_expand_tabs(phyto_string_span_t span,
                                        size_t tab_width);
bool phyto_string_find(phyto_string_span_t span,
                       phyto_string_span_t sub,
                       size_t* out_index);
bool phyto_string_find_in_range(phyto_string_span_t span,
                                phyto_string_span_t sub,
                                size_t start,
                                size_t end,
                                size_t* out_index);
bool phyto_string_is_alphanumeric(phyto_string_span_t span);
bool phyto_string_is_alphabetic(phyto_string_span_t span);
bool phyto_string_is_decimal(phyto_string_span_t span);
bool phyto_string_is_identifier(phyto_string_span_t span);
bool phyto_string_is_lowercase(phyto_string_span_t span);
bool phyto_string_is_printable(phyto_string_span_t span);
bool phyto_string_is_space(phyto_string_span_t span);
bool phyto_string_is_titlecase(phyto_string_span_t span);
bool phyto_string_is_uppercase(phyto_string_span_t span);
phyto_string_t phyto_string_join(phyto_string_span_t sep,
                                 phyto_string_vec_t args);
phyto_string_t phyto_string_left_justify(phyto_string_span_t span,
                                         size_t width,
                                         char fill);
phyto_string_vec_t phyto_string_partition(phyto_string_span_t span,
                                          phyto_string_span_t sep);
phyto_string_t phyto_string_remove_prefix(phyto_string_span_t span,
                                          phyto_string_span_t prefix);
phyto_string_t phyto_string_remove_suffix(phyto_string_span_t span,
                                          phyto_string_span_t suffix);
phyto_string_t phyto_string_replace(phyto_string_span_t span,
                                    phyto_string_span_t old,
                                    phyto_string_span_t new);
bool phyto_string_find_last(phyto_string_span_t span,
                            phyto_string_span_t sub,
                            size_t* out_index);
bool phyto_string_find_last_in_range(phyto_string_span_t span,
                                     phyto_string_span_t sub,
                                     size_t start,
                                     size_t end,
                                     size_t* out_index);
phyto_string_t phyto_string_right_justify(phyto_string_span_t span,
                                          size_t width,
                                          char fill);
phyto_string_vec_t phyto_string_partition_last(phyto_string_span_t span,
                                               phyto_string_span_t sep);
phyto_string_vec_t phyto_string_split(phyto_string_span_t span,
                                      phyto_string_span_t sep);
phyto_string_vec_t phyto_string_split_count(phyto_string_span_t span,
                                            phyto_string_span_t sep,
                                            size_t count);
phyto_string_vec_t phyto_string_split_reverse(phyto_string_span_t span,
                                              phyto_string_span_t sep);
phyto_string_vec_t phyto_string_split_reverse_count(phyto_string_span_t span,
                                                    phyto_string_span_t sep,
                                                    size_t count);
phyto_string_vec_t phyto_string_split_whitespace(phyto_string_span_t span);
phyto_string_span_t phyto_string_find_whitespace(phyto_string_span_t span);
phyto_string_vec_t phyto_string_split_lines(phyto_string_span_t span);
phyto_string_t phyto_string_strip(phyto_string_span_t span);
phyto_string_t phyto_string_strip_chars(phyto_string_span_t span,
                                        phyto_string_span_t chars);
bool phyto_string_span_contains(phyto_string_span_t span, char c);
phyto_string_t phyto_string_swap_case(phyto_string_span_t span);
phyto_string_t phyto_string_title(phyto_string_span_t span);
phyto_string_t phyto_string_upper(phyto_string_span_t span);
phyto_string_t phyto_string_zero_fill(phyto_string_span_t span, size_t width);

void phyto_string_free(phyto_string_t* str);
void phyto_string_vec_free(phyto_string_vec_t* vec);

#endif  // PHYTO_STRING_STRING_H_
