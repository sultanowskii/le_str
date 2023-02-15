#include <malloc.h>
#include <math.h>
#include <string.h>

#include "le_str.h"

#define MIN(a, b) (a <= b) ? (a) : (b)
#define MAX(a, b) (a >= b) ? (a) : (b)

struct le_str *le_str_create() {
    struct le_str *s = (struct le_str *)malloc(sizeof(struct le_str));

    s->data = (char *)calloc(DEFAULT_CHUNK_SIZE, sizeof(char));
    s->chunk_size = DEFAULT_CHUNK_SIZE;
    s->length = 0;

    return s;
}

struct le_str *le_str_create_with_cstr(char const *cstr) {
    struct le_str *s = le_str_create();

    __le_str_append_cstr(s, cstr);

    return s;
}

struct le_str *le_str_create_with_length(size_t length) {
    struct le_str *s = (struct le_str *)malloc(sizeof(struct le_str));
    // +1 magic not to worry about \0
    size_t chunk_size = MAX(DEFAULT_CHUNK_SIZE, length + 1);

    s->data = (char *)calloc(chunk_size, sizeof(char));
    s->chunk_size = DEFAULT_CHUNK_SIZE;
    s->length = 0;

    return s;
}

void le_str_destroy(struct le_str *s) {
    free(s->data);
    s->data = NULL;
    free(s);
}

inline int le_str_is_index_within_length(struct le_str const *s, size_t index) {
    return 0 <= index && 0 < s->chunk_size - 1;
}

inline char __unsafe_le_str_get_c(struct le_str const *s, size_t index) {
    return s->data[index];
}

inline char le_str_get_c(struct le_str const *s, size_t index) {
    if (!le_str_is_index_within_length(s, index))
        return 0xff;

    return s->data[index];
}

inline void __unsafe_le_str_set_c(struct le_str *s, size_t index, char c) {
    s->data[index] = c;
}

inline void le_str_set_c(struct le_str *s, size_t index, char c) {
    if (le_str_is_index_within_length(s, index))
        s->data[index] = c;
}

struct le_str *le_str_get_slice(struct le_str const *s, size_t start, size_t end) {
    if (start >= end)
        return (struct le_str *)0;

    struct le_str *sub_s = le_str_create();

    __le_str_append_ncstr(sub_s, s->data + start, end - start);

    return sub_s;
}

void __le_str_expand_upto(struct le_str *s, size_t expected_chunk_size) {
    if (s->chunk_size >= expected_chunk_size)
        return;

    while (s->chunk_size < expected_chunk_size)
        s->chunk_size *= 2;

    s->data = realloc(s->data, s->chunk_size);
}

void __le_str_append_cstr(struct le_str *s, char const *cstr) {
    size_t cstr_length = strlen(cstr);
    size_t result_cstr_length = s->length + cstr_length;

    if (result_cstr_length > s->chunk_size)
        __le_str_expand_upto(s, result_cstr_length);

    strcat(s->data + s->length, cstr);
    s->length = result_cstr_length;
}

void __le_str_append_ncstr(struct le_str *s, char const *cstr, size_t n) {
    size_t result_cstr_length = s->length + n;

    if (result_cstr_length > s->chunk_size)
        __le_str_expand_upto(s, result_cstr_length);

    strncat(s->data + s->length, cstr, n);
    s->length = result_cstr_length;
}

struct le_str *le_str_add(struct le_str const *first, struct le_str const *second) {
    struct le_str *new_s = le_str_create();

    __le_str_append_cstr(new_s, first->data);
    __le_str_append_cstr(new_s, second->data);

    new_s->length = first->length + second->length;
    return new_s;
}

void le_str_concat(struct le_str *s, struct le_str const *second) { __le_str_append_cstr(s, second->data); }

char *le_str_begin(struct le_str const *s) {
    if (s->length == 0) 
        return NULL;
    return &s->data[0];
}

char *le_str_end(struct le_str const *s) {
    if (s->length == 0) 
        return NULL;
    return &s->data[s->length];
}

void le_str_clear_data(struct le_str *s) {
    memset(s->data, 0, s->length);
    s->length = 0;
}

struct le_str *le_str_dup(struct le_str const *s) {
    return le_str_create_with_cstr(s->data);
}

struct le_str *le_str_get_reversed(struct le_str const *s) {
    struct le_str *reversed = le_str_create_with_length(s->length);
    size_t length = s->length;

    for (size_t i = 0; i < length; i++) {
        char symbol = __unsafe_le_str_get_c(s, length - i - 1);
        __unsafe_le_str_set_c(reversed, i, symbol);
    }

    reversed->length = s->length;

    return reversed;
}

BOOL le_str_is_valid(struct le_str const *s) {
    // corrupted length
    if (s->length != strlen(s->data)) {
        return FALSE;
    }

    // last symbol is not \0
    if (__unsafe_le_str_get_c(s, s->length) != '\0') {
        return FALSE;
    }

    return TRUE;
}

size_t le_str_index(struct le_str const *s, char c) {
    for (size_t i = 0; i < s->length; i++) {
        if (__unsafe_le_str_get_c(s, i) == c) {
            return i;
        }
    }
    return (size_t)-1;
}

size_t le_str_rindex(struct le_str const *s, char c) {
    for (size_t i = s->length; i > 0; i--) {
        size_t index = i - 1;
        if (__unsafe_le_str_get_c(s, index) == c) {
            return index;
        }
    }
    return (size_t)-1;
}
