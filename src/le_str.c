#include <malloc.h>
#include <math.h>
#include <string.h>

#include "le_str.h"

#define MIN(a, b) (a <= b) ? (a) : (b)
#define MAX(a, b) (a >= b) ? (a) : (b)

static void __le_str_append_cstr(struct le_str *s, char const *cstr);

struct le_str {
    size_t capacity;
    char *data;
};

struct le_str *le_str_create() {
    struct le_str *s = (struct le_str *)malloc(sizeof(struct le_str));

    s->data = (char *)calloc(DEFAULT_CAPACITY, sizeof(char));
    s->capacity = DEFAULT_CAPACITY;

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
    size_t capacity = MAX(DEFAULT_CAPACITY, length + 1);

    s->data = (char *)calloc(capacity, sizeof(char));
    s->capacity = capacity;

    return s;
}

void le_str_destroy(struct le_str *s) {
    free(s->data);
    s->data = NULL;
    free(s);
}

inline int le_str_is_index_within_capacity(struct le_str const *s, size_t index) {
    return 0 <= index && index < s->capacity - 1;
}

// Get le_str symbol on index without checks
static inline char __unsafe_le_str_get_at(struct le_str const *s, size_t index) {
    return s->data[index];
}

inline char le_str_get_at(struct le_str const *s, size_t index) {
    if (!le_str_is_index_within_capacity(s, index))
        return 0xff;

    return s->data[index];
}

// Set le_str symbol on index without checks
static inline void __unsafe_le_str_set_at(struct le_str *s, size_t index, char c) {
    s->data[index] = c;
}

inline void le_str_set_at(struct le_str *s, size_t index, char c) {
    if (le_str_is_index_within_capacity(s, index))
        s->data[index] = c;
}

// Expand string capacity up to `expected_chunk_size`
static void __le_str_expand_upto(struct le_str *s, size_t expected_chunk_size) {
    if (s->capacity >= expected_chunk_size)
        return;

    while (s->capacity < expected_chunk_size)
        s->capacity *= 2;

    s->data = realloc(s->data, s->capacity);
}

// Append cstr to the end of s
static void __le_str_append_cstr(struct le_str *s, char const *cstr) {
    size_t s_length = le_str_get_length(s);
    size_t result_cstr_length = s_length + strlen(cstr);

    if (result_cstr_length >= s->capacity)
        __le_str_expand_upto(s, result_cstr_length);

    strcat(s->data + s_length, cstr);
}

// Append n symbols of cstr to the end of s
static void __le_str_append_ncstr(struct le_str *s, char const *cstr, size_t n) {
    size_t s_length = le_str_get_length(s);
    size_t result_cstr_length = s_length + n;

    if (result_cstr_length >= s->capacity)
        __le_str_expand_upto(s, result_cstr_length);

    strncat(s->data + s_length, cstr, n);
}

struct le_str *le_str_add(struct le_str const *first, struct le_str const *second) {
    struct le_str *new_s = le_str_dup(first);

    __le_str_append_cstr(new_s, second->data);

    return new_s;
}

void le_str_concat(struct le_str *s, struct le_str const *second) {
    __le_str_append_cstr(s, second->data);
}

char *le_str_begin(struct le_str const *s) {
    if (s->capacity == 0) 
        return NULL;
    return &s->data[0];
}

char *le_str_end(struct le_str const *s) {
    if (s->capacity == 0) 
        return NULL;
    return &s->data[le_str_get_length(s)];
}

void le_str_clear_data(struct le_str *s) {
    memset(s->data, 0, s->capacity);
}

struct le_str *le_str_dup(struct le_str const *s) {
    return le_str_create_with_cstr(s->data);
}

struct le_str *le_str_get_reversed(struct le_str const *s) {
    size_t length = le_str_get_length(s);
    struct le_str *reversed = le_str_create_with_length(length);

    for (size_t i = 0; i < length; i++) {
        char symbol = __unsafe_le_str_get_at(s, length - i - 1);
        __unsafe_le_str_set_at(reversed, i, symbol);
    }

    return reversed;
}

BOOL le_str_is_valid(struct le_str const *s) {
    // last symbol is not \0
    if (__unsafe_le_str_get_at(s, le_str_get_length(s)) != '\0') {
        return FALSE;
    }

    return TRUE;
}

size_t le_str_index(struct le_str const *s, char c) {
    for (size_t i = 0; i < le_str_get_length(s); i++) {
        if (__unsafe_le_str_get_at(s, i) == c) {
            return i;
        }
    }
    return (size_t)-1;
}

size_t le_str_rindex(struct le_str const *s, char c) {
    for (size_t i = le_str_get_length(s); i > 0; i--) {
        size_t index = i - 1;
        if (__unsafe_le_str_get_at(s, index) == c) {
            return index;
        }
    }
    return (size_t)-1;
}

// Find index of substring (`sub`) in `s`.
static size_t __cstr_find(char const *s, size_t length, char const *sub, size_t sub_length) {
    size_t start_index = 0;
    size_t sub_i = 0;

    if (sub_length == 0)
        return 0;

    for (size_t i = 0; i < length; i++) {
        char s_symbol = s[i];
        char sub_symbol = sub[sub_i];

        if (s_symbol == sub_symbol) {
            if (sub_i == 0)
                start_index = i;
            sub_i++;
            if (sub_i >= sub_length)
                return start_index;
        } else {
            sub_i = 0;
        }
    }

    return -1;
}

size_t le_str_find(struct le_str const *s, struct le_str const *sub) {
    size_t start_index = 0;
    size_t sub_i = 0;

    size_t length = le_str_get_length(s);
    size_t sub_length = le_str_get_length(sub);

    if (sub_length == 0)
        return 0;

    for (size_t i = 0; i < length; i++) {
        char s_symbol = __unsafe_le_str_get_at(s, i);
        char sub_symbol = __unsafe_le_str_get_at(sub, sub_i);

        if (s_symbol == sub_symbol) {
            if (sub_i == 0)
                start_index = i;
            sub_i++;
            if (sub_i >= sub_length)
                return start_index;
        } else {
            sub_i = 0;
        }
    }

    return -1;
}

size_t le_str_find_n(struct le_str const *s, struct le_str const *sub, unsigned int n) {
    size_t start_index = 0;
    size_t sub_i = 0;

    unsigned int match_cntr = 0;

    size_t length = le_str_get_length(s);
    size_t sub_length = le_str_get_length(sub);

    if (sub_length == 0)
        return 0;

    for (size_t i = 0; i < length; i++) {
        char s_symbol = __unsafe_le_str_get_at(s, i);
        char sub_symbol = __unsafe_le_str_get_at(sub, sub_i);

        if (s_symbol == sub_symbol) {
            if (sub_i == 0)
                start_index = i;
            sub_i++;
            if (sub_i >= sub_length) {
                if (++match_cntr == n)
                    return start_index;
                sub_i = 0;
            }
        } else {
            sub_i = 0;
        }
    }

    return -1;
}

struct le_str *le_str_slice(struct le_str const *s, size_t start, size_t end) {
    if (!le_str_is_index_within_capacity(s, start) || !le_str_is_index_within_capacity(s, end)) {
        return (struct le_str *)-1;
    }

    if (start > end) {
        return (struct le_str *)-2;
    }

    struct le_str *new_s = le_str_create_with_length(end - start + 1);

    for (size_t i = start; i <= end; i++) {
        size_t new_s_index = i - start;
        char tmp = __unsafe_le_str_get_at(s, i);
        __unsafe_le_str_set_at(new_s, new_s_index, tmp); 
    }

    return new_s;
}

struct le_str **le_str_split(struct le_str const *s, struct le_str const *delimiter) {
    // TODO: change to vector
    size_t token_nmb = le_str_get_length(s) + 2;
    struct le_str **tokens = (struct le_str**)calloc(token_nmb, sizeof(struct le_str *));
    struct le_str **token_ptr = tokens;

    size_t length = le_str_get_length(s);
    size_t delimiter_length = le_str_get_length(delimiter);
    size_t data_i = 0;
    size_t delimiter_index = 0;
    size_t prev_delimiter_end_index = 0;

    if (delimiter_length == 0) {
        *token_ptr = le_str_dup(s);
        return tokens;
    }

    while (delimiter_index < length) {
        delimiter_index = __cstr_find(
            s->data + prev_delimiter_end_index,
            length - prev_delimiter_end_index,
            delimiter->data,
            delimiter_length
        );
        if (delimiter_index == -1) {
            *(token_ptr++) = le_str_slice(s, prev_delimiter_end_index, length);
            break;
        }
        delimiter_index += prev_delimiter_end_index;
        if (prev_delimiter_end_index == delimiter_index) {
            *(token_ptr++) = le_str_create_with_cstr("");
        } else {
            *(token_ptr++) = le_str_slice(s, prev_delimiter_end_index, delimiter_index - 1);
        }
        prev_delimiter_end_index = delimiter_index + delimiter_length;
    }
    
    return tokens;
}

struct le_str *le_str_replace(struct le_str const *s, struct le_str const *old, struct le_str const *new) {
    struct le_str **tokens = le_str_split(s, old);
    struct le_str *result = le_str_create();

    struct le_str **token_ptr = tokens;

    while (*token_ptr != NULL) {
        le_str_concat(result, *token_ptr);
        le_str_destroy(*token_ptr);
        if (*(++token_ptr) != NULL) {
            le_str_concat(result, new);
        }
    }

    free(tokens);

    return result;
}

inline const char *le_str_get_raw(struct le_str const *s) {
    return s->data;
}

inline size_t le_str_get_capacity(struct le_str const *s) {
    return s->capacity;
}

inline size_t le_str_get_length(struct le_str const *s) {
    return strlen(s->data);
}
