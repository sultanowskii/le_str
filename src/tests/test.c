#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "le_str.h"

int err_cntr = 0;
int assert_cntr = 0;

#define ASSERT(x) { \
    if (!(x)) { \
        printf("[!] Test #%d failed (%s, %s(), line %d).\n", __COUNTER__ + 1, __FILE__, __func__, __LINE__); \
        err_cntr++; \
    } \
    assert_cntr++; \
}

void test_create() {
    struct le_str *s = le_str_create();
    le_str_destroy(s);
}

void test_create_with_cstr() {
    char const *hello = "Hello ";
    char const *world = "world!";
    char *helloworld = calloc(64, sizeof(char));
    strcpy(helloworld, hello);
    strcat(helloworld, world);

    struct le_str *s1 = le_str_create_with_cstr(hello);
    ASSERT(s1->length == strlen(hello))
    ASSERT(strcmp(s1->data, hello) == 0)
    ASSERT(le_str_is_valid(s1))

    struct le_str *s2 = le_str_create_with_cstr(world);
    ASSERT(s2->length == strlen(world))
    ASSERT(strcmp(s2->data, world) == 0)
    ASSERT(le_str_is_valid(s2))

    struct le_str *s3 = le_str_add(s1, s2);
    ASSERT(s3->length == strlen(hello) + strlen(world))
    ASSERT(strcmp(s3->data, helloworld) == 0)
    ASSERT(le_str_is_valid(s3))

    le_str_destroy(s1);
    le_str_destroy(s2);
    le_str_destroy(s3);
    free(helloworld);
}

void test_is_index_within_length() {
    struct le_str *s = le_str_create_with_cstr("abc");

    ASSERT(le_str_is_index_within_length(s, 0) == 1)
    ASSERT(le_str_is_index_within_length(s, 1) == 1)
    ASSERT(le_str_is_index_within_length(s, 2) == 1)
    // TODO: think about this inconvenience
    ASSERT(le_str_is_index_within_length(s, DEFAULT_CHUNK_SIZE) == 0)

    le_str_destroy(s);
}

void test_pointers() {
    struct le_str *s = le_str_create_with_cstr("Last chance to look at me, Hector");

    ASSERT(le_str_begin(s) == s->data)
    ASSERT(le_str_end(s) == s->data + s->length)

    le_str_destroy(s);
}

void test_indexes() {
    struct le_str *s = le_str_create_with_cstr("Hello!");

    ASSERT(le_str_index(s, 'l') == 2)
    ASSERT(le_str_rindex(s, 'l') == 3)
    ASSERT(le_str_index(s, 'Q') == (size_t)-1)
    ASSERT(le_str_rindex(s, 'Q') == (size_t)-1)

    le_str_destroy(s);
}

void test_clear_data() {
    struct le_str *s = le_str_create_with_cstr("Tanki online");

    le_str_clear_data(s);
    ASSERT(s->length == 0);
    ASSERT(s->data[0] == 0);

    le_str_destroy(s);
}

void test_get_c() {
    char *cstr = "HOLY SHIT I'VE JUST HIT A CLIP";
    struct le_str *s = le_str_create_with_cstr(cstr);

    char c_from_method = le_str_get_c(s, 2);
    char c_from_data = s->data[2];
    char c_from_orig = cstr[2];
    ASSERT(c_from_method == c_from_orig)
    ASSERT(c_from_method == c_from_data)

    le_str_destroy(s);
}

void test_set_c() {
    char *cstr = "HOLY SHIT I'VE JUST HIT A CLIP";
    struct le_str *s = le_str_create_with_cstr(cstr);

    le_str_set_c(s, 7, '*');
    char c = le_str_get_c(s, 7);
    ASSERT(c == '*')

    le_str_destroy(s);
}

void test_dup() {
    struct le_str *s = le_str_create_with_cstr("Me when the");

    struct le_str *s_copy = le_str_dup(s);
    ASSERT(s_copy->length == s->length)
    ASSERT(strcmp(s_copy->data, s->data) == 0)
    ASSERT(le_str_is_valid(s_copy))

    le_str_destroy(s);
    le_str_destroy(s_copy);
}

void test_reverse() {
    struct le_str *s = le_str_create_with_cstr("Fortnite balls");

    struct le_str *s_reversed = le_str_get_reversed(s);
    ASSERT(s_reversed->length == s->length)
    for (size_t i = 0; i < s->length; i++) {
        ASSERT(le_str_get_c(s, i) == le_str_get_c(s_reversed, s_reversed->length - i - 1))
    }
    ASSERT(le_str_is_valid(s_reversed))

    le_str_destroy(s);
    le_str_destroy(s_reversed);
}

void test_find() {
    struct le_str *empty = le_str_create_with_cstr("");
    struct le_str *ab = le_str_create_with_cstr("ab");
    struct le_str *ddabdabab = le_str_create_with_cstr("ddabdabab");
    struct le_str *cool = le_str_create_with_cstr("cool");

    ASSERT(le_str_find(ddabdabab, ab) == 2);
    ASSERT(le_str_find(ddabdabab, cool) == (size_t)-1);
    ASSERT(le_str_find_n(ddabdabab, ab, 2) == 5);
    ASSERT(le_str_find(ddabdabab, empty) == 0);
    ASSERT(le_str_find(ddabdabab, ddabdabab) == 0);

    le_str_destroy(ab);
    le_str_destroy(ddabdabab);
    le_str_destroy(cool);
    le_str_destroy(empty);
}

void test_slice() {
    struct le_str *s123456789 = le_str_create_with_cstr("123456789");
    struct le_str *slice1 = le_str_slice(s123456789, 2, 4);
    struct le_str *slice2 = le_str_slice(s123456789, 2, 100);
    struct le_str *slice3 = le_str_slice(s123456789, 5, 4);
    struct le_str *slice4 = le_str_slice(s123456789, 8, 8);

    ASSERT(strcmp(slice1->data, "345") == 0)
    ASSERT(slice2 == (struct le_str *)-1)
    ASSERT(slice3 == (struct le_str *)-2)
    ASSERT(strcmp(slice4->data, "9") == 0)

    le_str_destroy(s123456789);
    le_str_destroy(slice1);
    le_str_destroy(slice4);
}

int main() {
    puts("Running tests...\n");

    test_create();
    test_create_with_cstr();
    test_is_index_within_length();
    test_pointers();
    test_indexes();
    test_clear_data();
    test_dup();
    test_get_c();
    test_set_c();
    test_reverse();
    test_find();
    test_slice();

    printf("\nFinished. %d/%d tests passed.\n", assert_cntr - err_cntr, assert_cntr);
    return 0;
}
