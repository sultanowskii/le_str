#include <malloc.h>
#include <math.h>
#include <string.h>

#include "le_str.h"

struct le_str *le_str_create() {
    struct le_str *s = (struct le_str *)malloc(sizeof(struct le_str));

    s->data = (char *)malloc(DEFAULT_CHUNK_SIZE);
    s->chunk_size = DEFAULT_CHUNK_SIZE;
    s->length = 0;

    return s;
}

struct le_str *le_str_create_with_cstr(char const *cstr) {
    struct le_str *s = le_str_create();

    __le_str_append_cstr(s, cstr);

    return s;
}

void le_str_destroy(struct le_str *s) {
    free(s->data);
    s->data = NULL;
    free(s);
}

inline int le_str_is_index_within_length(struct le_str const *s, unsigned int index) {
    return 0 <= index && 0 < s->chunk_size - 1;
}

inline char le_str_get_c(struct le_str const *s, unsigned int index) {
    if (!le_str_is_index_within_length(s, index))
        return 0xff;

    return s->data[index];
}

inline void le_str_set_c(struct le_str *s, unsigned int index, char c) {
    if (le_str_is_index_within_length(s, index))
        s->data[index] = c;
}

struct le_str *le_str_get_slice(struct le_str const *s, unsigned int start, unsigned int end) {
    if (start >= end)
        return (struct le_str *)0;

    struct le_str *sub_s = le_str_create();

    __le_str_append_ncstr(sub_s, s->data + start, end - start);

    return sub_s;
}

void __le_str_expand_upto(struct le_str *s, unsigned int expected_chunk_size) {
    if (s->chunk_size >= expected_chunk_size)
        return;

    while (s->chunk_size < expected_chunk_size)
        s->chunk_size *= 2;

    s->data = realloc(s->data, s->chunk_size);
}

void __le_str_append_cstr(struct le_str *s, char const *cstr) {
    unsigned int cstr_length = strlen(cstr);
    unsigned int result_cstr_length = s->length + cstr_length;

    if (result_cstr_length > s->chunk_size)
        __le_str_expand_upto(s, result_cstr_length);

    strcat(s->data + s->length, cstr);
    s->length = result_cstr_length;
}

void __le_str_append_ncstr(struct le_str *s, char const *cstr, unsigned int n) {
    unsigned int result_cstr_length = s->length + n;

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
