#include "phyto/string/string.h"

#include <nonstd/ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

PHYTO_COLLECTIONS_DYNAMIC_ARRAY_IMPL(phyto_string, char);
PHYTO_COLLECTIONS_DYNAMIC_ARRAY_IMPL(phyto_string_vec, phyto_string_t);

static int32_t charcmp(char a, char b) {
  return (a > b) - (a < b);
}

static void charprn(char c, FILE* fp) {
  fputc(c, fp);
}

static const phyto_string_callbacks_t string_callbacks = {
    .compare_cb = charcmp,
    .print_cb = charprn,
};

static int32_t phyto_string_compare_internal(phyto_string_t a,
                                             phyto_string_t b) {
  return phyto_string_compare(phyto_string_as_span(a), phyto_string_as_span(b));
}

void phyto_string_print_nosep(phyto_string_t string, FILE* stream) {
  phyto_string_print(string, stream, "");
}

const phyto_string_vec_callbacks_t phyto_string_vec_callbacks = {
    .compare_cb = phyto_string_compare_internal,
    .print_cb = phyto_string_print_nosep,
    .copy_cb = phyto_string_copy,
    .free_cb = phyto_string_free,
};

phyto_string_t phyto_string_new(void) {
  return phyto_string_init(&string_callbacks);
}

phyto_string_t phyto_string_own(phyto_string_span_t span) {
  return phyto_string_init_from_span(&string_callbacks, span);
}

phyto_string_t phyto_string_from_c(const char* str) {
  return phyto_string_own(phyto_string_span_from_c(str));
}

phyto_string_span_t phyto_string_span_from_c(const char* str) {
  return phyto_string_span_from_array(str, strlen(str));
}

bool phyto_string_span_equal(phyto_string_span_t a, phyto_string_span_t b) {
  return phyto_string_compare(a, b) == 0;
}

phyto_string_t phyto_string_from_sprintf(const char* format, ...) {
  va_list args;
  va_start(args, format);
  int length = vsnprintf(NULL, 0, format, args);
  va_end(args);
  phyto_string_t str = phyto_string_new();
  phyto_string_reserve(&str, (size_t)length + 1);
  va_start(args, format);
  vsnprintf(str.data, (size_t)length + 1, format, args);
  va_end(args);
  str.size = length;
  return str;
}

int32_t phyto_string_compare(phyto_string_span_t lhs, phyto_string_span_t rhs) {
  if (lhs.size < rhs.size) {
    return -1;
  }
  if (lhs.size > rhs.size) {
    return 1;
  }
  return memcmp(lhs.begin, rhs.begin, lhs.size);
}

bool phyto_string_append_fill(phyto_string_t* string, size_t count, char fill) {
  for (size_t i = 0; i < count; ++i) {
    if (!phyto_string_append(string, fill)) {
      return false;
    }
  }
  return true;
}

bool phyto_string_append_c(phyto_string_t* string, const char* cstr) {
  return phyto_string_extend(string, phyto_string_span_from_c(cstr));
}

void phyto_string_span_print_to(phyto_string_span_t span, FILE* stream) {
  fwrite(span.begin, 1, span.size, stream);
}

phyto_string_conversion_result_i64_t phyto_string_to_i64(
    phyto_string_span_t span) {
  phyto_string_conversion_result_i64_t result = {0};
  phyto_string_t temp = phyto_string_own(span);
  phyto_string_append(&temp, '\0');
  char* endptr;
  result.value = strtol(temp.data, &endptr, 10);
  result.success = (endptr != temp.data);
  phyto_string_free(&temp);
  return result;
}

phyto_string_t phyto_string_capitalize(phyto_string_span_t span) {
  phyto_string_t result = phyto_string_new();
  phyto_string_reserve(&result, span.size);
  for (size_t i = 0; i < span.size; ++i) {
    char c = span.begin[i];
    if (i == 0) {
      c = nonstd_toupper(c);
    } else {
      c = nonstd_tolower(c);
    }
    phyto_string_append(&result, c);
  }
  return result;
}

phyto_string_t phyto_string_center(phyto_string_span_t span,
                                   size_t width,
                                   char fill) {
  phyto_string_t result = phyto_string_new();
  if (span.size >= width) {
    return phyto_string_own(span);
  }
  size_t left_padding = (width - span.size) / 2;
  size_t right_padding = width - span.size - left_padding;
  for (size_t i = 0; i < left_padding; ++i) {
    phyto_string_append(&result, fill);
  }
  phyto_string_extend(&result, span);
  for (size_t i = 0; i < right_padding; ++i) {
    phyto_string_append(&result, fill);
  }
  return result;
}

size_t phyto_string_count_sub(phyto_string_span_t span,
                              phyto_string_span_t sub) {
  return phyto_string_count_sub_in_range(span, sub, 0, span.size);
}

size_t phyto_string_count_sub_in_range(phyto_string_span_t span,
                                       phyto_string_span_t sub,
                                       size_t start,
                                       size_t end) {
  if (sub.size == 0) {
    return 0;
  }
  if (start > end) {
    return 0;
  }
  if (end - start < sub.size) {
    return 0;
  }
  size_t count = 0;
  size_t sub_size = sub.size;
  for (size_t i = start; i <= end - sub_size; ++i) {
    if (memcmp(span.begin + i, sub.begin, sub_size) == 0) {
      ++count;
    }
  }
  return count;
}

bool phyto_string_ends_with(phyto_string_span_t span, phyto_string_span_t sub) {
  if (span.size < sub.size) {
    return false;
  }
  if (sub.size == 0) {
    return false;
  }
  return memcmp(span.begin + span.size - sub.size, sub.begin, sub.size) == 0;
}

bool phyto_string_starts_with(phyto_string_span_t span,
                              phyto_string_span_t sub) {
  if (span.size < sub.size) {
    return false;
  }
  if (sub.size == 0) {
    return false;
  }
  return memcmp(span.begin, sub.begin, sub.size) == 0;
}

phyto_string_t phyto_string_expand_tabs(phyto_string_span_t span,
                                        size_t tab_width) {
  if (tab_width == 0) {
    return phyto_string_replace(span, phyto_string_span_from_c("\t"),
                                phyto_string_span_empty());
  }
  phyto_string_t result = phyto_string_new();
  // if no tabs, it will be the same length
  // if there are tabs, it will be longer but the vec will just resize
  phyto_string_reserve(&result, span.size);
  for (size_t i = 0; i < span.size; ++i) {
    char c = span.begin[i];
    if (c == '\t') {
      size_t num_spaces = tab_width - (result.size % tab_width);
      for (size_t j = 0; j < num_spaces; ++j) {
        phyto_string_append(&result, ' ');
      }
    } else {
      phyto_string_append(&result, c);
    }
  }
  return result;
}

bool phyto_string_find(phyto_string_span_t span,
                       phyto_string_span_t sub,
                       size_t* out_index) {
  return phyto_string_find_in_range(span, sub, 0, span.size, out_index);
}

bool phyto_string_find_in_range(phyto_string_span_t span,
                                phyto_string_span_t sub,
                                size_t start,
                                size_t end,
                                size_t* out_index) {
  size_t sub_size = sub.size;
  for (size_t i = start; i < end - sub_size; ++i) {
    if (memcmp(span.begin + i, sub.begin, sub_size) == 0) {
      if (out_index != NULL) {
        *out_index = i;
      }
      return true;
    }
  }
  return false;
}

bool phyto_string_is_alphanumeric(phyto_string_span_t span) {
  if (span.size == 0) {
    return false;
  }
  for (size_t i = 0; i < span.size; ++i) {
    if (!nonstd_isalnum(span.begin[i])) {
      return false;
    }
  }
  return true;
}

bool phyto_string_is_alphabetic(phyto_string_span_t span) {
  if (span.size == 0) {
    return false;
  }
  for (size_t i = 0; i < span.size; ++i) {
    if (!nonstd_isalpha(span.begin[i])) {
      return false;
    }
  }
  return true;
}

bool phyto_string_is_decimal(phyto_string_span_t span) {
  if (span.size == 0) {
    return false;
  }
  for (size_t i = 0; i < span.size; ++i) {
    if (!nonstd_isdigit(span.begin[i])) {
      return false;
    }
  }
  return true;
}

bool phyto_string_is_identifier(phyto_string_span_t span) {
  if (span.size == 0) {
    return false;
  }
  if (!nonstd_isalpha(span.begin[0]) && span.begin[0] != '_') {
    return false;
  }
  for (size_t i = 1; i < span.size; ++i) {
    if (!nonstd_isalnum(span.begin[i]) && span.begin[i] != '_') {
      return false;
    }
  }
  return true;
}

bool phyto_string_is_lowercase(phyto_string_span_t span) {
  bool found = false;
  for (size_t i = 0; i < span.size; ++i) {
    if (nonstd_isupper(span.begin[i])) {
      return false;
    }
    if (nonstd_islower(span.begin[i])) {
      found = true;
    }
  }
  return found;
}

bool phyto_string_is_printable(phyto_string_span_t span) {
  if (span.size == 0) {
    return false;
  }
  for (size_t i = 0; i < span.size; ++i) {
    if (!nonstd_isprint(span.begin[i])) {
      return false;
    }
  }
  return true;
}

bool phyto_string_is_space(phyto_string_span_t span) {
  if (span.size == 0) {
    return false;
  }
  for (size_t i = 0; i < span.size; ++i) {
    if (!nonstd_isspace(span.begin[i])) {
      return false;
    }
  }
  return true;
}

bool phyto_string_is_titlecase(phyto_string_span_t span) {
  if (span.size == 0) {
    return false;
  }
  bool was_upper = false;
  bool was_lower = false;
  for (size_t i = 0; i < span.size; ++i) {
    if (nonstd_isupper(span.begin[i])) {
      if (was_upper || was_lower) {
        return false;
      }
      was_upper = true;
      was_lower = false;
    } else if (nonstd_islower(span.begin[i])) {
      was_lower = true;
      was_upper = false;
    } else {
      was_upper = false;
      was_lower = false;
    }
  }
  return true;
}

bool phyto_string_is_uppercase(phyto_string_span_t span) {
  bool found = false;
  if (span.size == 0) {
    return false;
  }
  for (size_t i = 0; i < span.size; ++i) {
    if (nonstd_islower(span.begin[i])) {
      return false;
    }
    if (nonstd_isupper(span.begin[i])) {
      found = true;
    }
  }
  return found;
}

phyto_string_t phyto_string_join(phyto_string_span_t sep,
                                 phyto_string_vec_t args) {
  phyto_string_t result = phyto_string_new();
  for (size_t i = 0; i < args.size; ++i) {
    phyto_string_extend(&result, phyto_string_as_span(args.data[i]));
    if (i < args.size - 1) {
      phyto_string_extend(&result, sep);
    }
  }
  return result;
}

phyto_string_t phyto_string_left_justify(phyto_string_span_t span,
                                         size_t width,
                                         char fill) {
  phyto_string_t result = phyto_string_new();
  size_t size = span.size;
  if (size >= width) {
    return phyto_string_own(span);
  }
  phyto_string_reserve(&result, width);
  phyto_string_extend(&result, span);
  phyto_string_append_fill(&result, width - size, fill);
  return result;
}

phyto_string_vec_t phyto_string_partition(phyto_string_span_t span,
                                          phyto_string_span_t sep) {
  phyto_string_vec_t result =
      phyto_string_vec_init(&phyto_string_vec_callbacks);
  size_t sep_pos;
  if (!phyto_string_find(span, sep, &sep_pos)) {
    // return (view, "", "")
    phyto_string_vec_append(&result, phyto_string_own(span));
    phyto_string_vec_append(&result, phyto_string_new());
    phyto_string_vec_append(&result, phyto_string_new());
    return result;
  }
  // return (view[:sep_pos], sep, view[sep_pos + sep.size:])
  phyto_string_vec_append(
      &result, phyto_string_own(phyto_string_span_subspan(span, 0, sep_pos)));
  phyto_string_vec_append(&result, phyto_string_own(sep));
  phyto_string_vec_append(&result, phyto_string_own(phyto_string_span_subspan(
                                       span, sep_pos + sep.size, span.size)));
  return result;
}

phyto_string_t phyto_string_remove_prefix(phyto_string_span_t span,
                                          phyto_string_span_t prefix) {
  if (!phyto_string_starts_with(span, prefix)) {
    return phyto_string_own(span);
  }
  return phyto_string_own(
      phyto_string_span_subspan(span, prefix.size, span.size));
}

phyto_string_t phyto_string_remove_suffix(phyto_string_span_t span,
                                          phyto_string_span_t suffix) {
  if (!phyto_string_ends_with(span, suffix)) {
    return phyto_string_own(span);
  }
  return phyto_string_own(
      phyto_string_span_subspan(span, 0, span.size - suffix.size));
}

phyto_string_t phyto_string_replace(phyto_string_span_t span,
                                    phyto_string_span_t old,
                                    phyto_string_span_t new) {
  phyto_string_t result = phyto_string_new();
  size_t pos = 0;
  while (true) {
    size_t next_pos;
    if (!phyto_string_find_in_range(span, old, pos, span.size, &next_pos)) {
      phyto_string_extend(&result,
                          phyto_string_span_subspan(span, pos, span.size));
      return result;
    }
    phyto_string_extend(&result,
                        phyto_string_span_subspan(span, pos, next_pos));
    phyto_string_extend(&result, new);
    pos = next_pos + old.size;
  }
}

bool phyto_string_find_last(phyto_string_span_t span,
                            phyto_string_span_t sub,
                            size_t* out_index) {
  return phyto_string_find_last_in_range(span, sub, 0, span.size, out_index);
}

bool phyto_string_find_last_in_range(phyto_string_span_t span,
                                     phyto_string_span_t sub,
                                     size_t start,
                                     size_t end,
                                     size_t* out_index) {
  size_t index = end;
  while (true) {
    size_t pos;
    if (!phyto_string_find_in_range(span, sub, index - sub.size, index, &pos)) {
      return false;
    }
    index = pos;
    if (index == start) {
      return false;
    }
    index -= sub.size;
    if (phyto_string_span_equal(
            span, phyto_string_span_subspan(span, index, sub.size))) {
      *out_index = index;
      return true;
    }
  }
}

phyto_string_t phyto_string_right_justify(phyto_string_span_t span,
                                          size_t width,
                                          char fill) {
  phyto_string_t result = phyto_string_new();
  size_t size = span.size;
  if (size >= width) {
    return phyto_string_own(span);
  }
  phyto_string_reserve(&result, width);
  phyto_string_append_fill(&result, width - size, fill);
  phyto_string_extend(&result, span);
  return result;
}

phyto_string_vec_t phyto_string_partition_last(phyto_string_span_t span,
                                               phyto_string_span_t sep) {
  phyto_string_vec_t result =
      phyto_string_vec_init(&phyto_string_vec_callbacks);
  size_t sep_pos;
  if (!phyto_string_find_last(span, sep, &sep_pos)) {
    // return (view, "", "")
    phyto_string_vec_append(&result, phyto_string_own(span));
    phyto_string_vec_append(&result, phyto_string_new());
    phyto_string_vec_append(&result, phyto_string_new());
    return result;
  }
  // return (view[:sep_pos], sep, view[sep_pos + sep.size:])
  phyto_string_vec_append(
      &result, phyto_string_own(phyto_string_span_subspan(span, 0, sep_pos)));
  phyto_string_vec_append(&result, phyto_string_own(sep));
  phyto_string_vec_append(&result, phyto_string_own(phyto_string_span_subspan(
                                       span, sep_pos + sep.size, span.size)));
  return result;
}

phyto_string_vec_t phyto_string_split(phyto_string_span_t span,
                                      phyto_string_span_t sep) {
  phyto_string_vec_t result =
      phyto_string_vec_init(&phyto_string_vec_callbacks);
  size_t sep_pos;
  while (true) {
    if (!phyto_string_find(span, sep, &sep_pos)) {
      phyto_string_vec_append(&result, phyto_string_own(span));
      return result;
    }
    phyto_string_vec_append(
        &result, phyto_string_own(phyto_string_span_subspan(span, 0, sep_pos)));
    span = phyto_string_span_subspan(span, sep_pos + sep.size, span.size);
  }
}

phyto_string_vec_t phyto_string_split_count(phyto_string_span_t span,
                                            phyto_string_span_t sep,
                                            size_t count) {
  phyto_string_vec_t result =
      phyto_string_vec_init(&phyto_string_vec_callbacks);
  size_t sep_pos;
  while (count > 0) {
    if (!phyto_string_find(span, sep, &sep_pos)) {
      break;
    }
    phyto_string_vec_append(
        &result, phyto_string_own(phyto_string_span_subspan(span, 0, sep_pos)));
    span = phyto_string_span_subspan(span, sep_pos + sep.size, span.size);
    count--;
  }
  phyto_string_vec_append(&result, phyto_string_own(span));
  return result;
}

phyto_string_vec_t phyto_string_split_reverse(phyto_string_span_t span,
                                              phyto_string_span_t sep) {
  phyto_string_vec_t result =
      phyto_string_vec_init(&phyto_string_vec_callbacks);
  size_t sep_pos;
  while (true) {
    if (!phyto_string_find_last(span, sep, &sep_pos)) {
      phyto_string_vec_append(&result, phyto_string_own(span));
      return result;
    }
    phyto_string_vec_append(&result, phyto_string_own(phyto_string_span_subspan(
                                         span, sep_pos + sep.size, span.size)));
    span = phyto_string_span_subspan(span, 0, sep_pos);
  }
}

phyto_string_vec_t phyto_string_split_reverse_count(phyto_string_span_t span,
                                                    phyto_string_span_t sep,
                                                    size_t count) {
  phyto_string_vec_t result =
      phyto_string_vec_init(&phyto_string_vec_callbacks);
  size_t sep_pos;
  while (count > 0) {
    if (!phyto_string_find_last(span, sep, &sep_pos)) {
      break;
    }
    phyto_string_vec_append(&result, phyto_string_own(phyto_string_span_subspan(
                                         span, sep_pos + sep.size, span.size)));
    span = phyto_string_span_subspan(span, 0, sep_pos);
    count--;
  }
  phyto_string_vec_append(&result, phyto_string_own(span));
  return result;
}

phyto_string_vec_t phyto_string_split_whitespace(phyto_string_span_t span) {
  phyto_string_vec_t result =
      phyto_string_vec_init(&phyto_string_vec_callbacks);
  while (true) {
    phyto_string_span_t sep = phyto_string_find_whitespace(span);
    if (sep.size == 0) {
      phyto_string_vec_append(&result, phyto_string_own(span));
      return result;
    }
    phyto_string_vec_append(&result, phyto_string_own(phyto_string_span_subspan(
                                         span, 0, sep.begin - span.begin)));
    span = phyto_string_span_subspan(span, sep.end - span.begin, span.size);
  }
}

phyto_string_span_t phyto_string_find_whitespace(phyto_string_span_t span) {
  size_t i;
  for (i = 0; i < span.size; i++) {
    if (nonstd_isspace(span.begin[i])) {
      size_t len = 1;
      while (i + len < span.size && nonstd_isspace(span.begin[i + len])) {
        len++;
      }
      return phyto_string_span_subspan(span, i, len);
    }
  }
  return phyto_string_span_empty();
}

phyto_string_vec_t phyto_string_split_lines(phyto_string_span_t span) {
  phyto_string_vec_t result =
      phyto_string_vec_init(&phyto_string_vec_callbacks);
  size_t newline_pos;
  while (true) {
    if (!phyto_string_find(span, phyto_string_span_from_c("\r\n"),
                           &newline_pos) &&
        !phyto_string_find(span, phyto_string_span_from_c("\n"),
                           &newline_pos)) {
      phyto_string_vec_append(&result, phyto_string_own(span));
      return result;
    }
    phyto_string_vec_append(&result, phyto_string_own(phyto_string_span_subspan(
                                         span, 0, newline_pos)));
    span = phyto_string_span_subspan(span, newline_pos + 1, span.size);
  }
}

phyto_string_t phyto_string_strip(phyto_string_span_t span) {
  size_t i;
  for (i = 0; i < span.size; i++) {
    if (!nonstd_isspace(span.begin[i])) {
      break;
    }
  }
  size_t j;
  for (j = span.size - 1; j > i; j--) {
    if (!nonstd_isspace(span.begin[j])) {
      break;
    }
  }
  return phyto_string_own(phyto_string_span_subspan(span, i, j + 1));
}

phyto_string_t phyto_string_strip_chars(phyto_string_span_t span,
                                        phyto_string_span_t chars) {
  size_t i;
  for (i = 0; i < span.size; i++) {
    if (!phyto_string_span_contains(chars, span.begin[i])) {
      break;
    }
  }
  size_t j;
  for (j = span.size - 1; j > i; j--) {
    if (!phyto_string_span_contains(chars, span.begin[j])) {
      break;
    }
  }
  return phyto_string_own(phyto_string_span_subspan(span, i, j + 1));
}

bool phyto_string_span_contains(phyto_string_span_t span, char c) {
  for (size_t i = 0; i < span.size; i++) {
    if (span.begin[i] == c) {
      return true;
    }
  }
  return false;
}

phyto_string_t phyto_string_swap_case(phyto_string_span_t span) {
  phyto_string_t result = phyto_string_new();
  for (size_t i = 0; i < span.size; i++) {
    char c = span.begin[i];
    if (nonstd_isupper(c)) {
      c = nonstd_tolower(c);
    } else if (nonstd_islower(c)) {
      c = nonstd_toupper(c);
    }
    phyto_string_append(&result, c);
  }
  return result;
}

phyto_string_t phyto_string_title(phyto_string_span_t span) {
  phyto_string_t result = phyto_string_new();
  bool was_upper = false;
  for (size_t i = 0; i < span.size; ++i) {
    char c = span.begin[i];
    if (was_upper) {
      c = nonstd_tolower(c);
      was_upper = false;
    } else if (nonstd_isalpha(c)) {
      c = nonstd_toupper(c);
      was_upper = true;
    } else {
      was_upper = false;
    }
    phyto_string_append(&result, c);
  }
  return result;
}

phyto_string_t phyto_string_upper(phyto_string_span_t span) {
  phyto_string_t result = phyto_string_new();
  phyto_string_reserve(&result, span.size);
  for (size_t i = 0; i < span.size; ++i) {
    phyto_string_append(&result, nonstd_toupper(span.begin[i]));
  }
  return result;
}

phyto_string_t phyto_string_zero_fill(phyto_string_span_t span, size_t width) {
  phyto_string_t result = phyto_string_new();
  phyto_string_reserve(&result, width);
  if (span.size > 0 && (span.begin[0] == '+' || span.begin[0] == '-')) {
    phyto_string_append(&result, span.begin[0]);
    span = phyto_string_span_subspan(span, 1, span.size);
  }
  phyto_string_append_fill(&result, width - span.size, '0');
  phyto_string_extend(&result, span);
  return result;
}
