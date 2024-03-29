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
    ASSERT(le_str_get_length(s1) == strlen(hello))
    ASSERT(strcmp(le_str_get_raw(s1), hello) == 0)
    ASSERT(le_str_is_valid(s1))

    struct le_str *s2 = le_str_create_with_cstr(world);
    ASSERT(le_str_get_length(s2) == strlen(world))
    ASSERT(strcmp(le_str_get_raw(s2), world) == 0)
    ASSERT(le_str_is_valid(s2))

    struct le_str *s3 = le_str_add(s1, s2);
    ASSERT(le_str_get_length(s3) == strlen(hello) + strlen(world))
    ASSERT(strcmp(le_str_get_raw(s3), helloworld) == 0)
    ASSERT(le_str_is_valid(s3))

    le_str_destroy(s1);
    le_str_destroy(s2);
    le_str_destroy(s3);
    free(helloworld);
}

void test_is_index_within_length() {
    struct le_str *s = le_str_create_with_cstr("abc");

    ASSERT(le_str_is_index_within_capacity(s, 0) == 1)
    ASSERT(le_str_is_index_within_capacity(s, 1) == 1)
    ASSERT(le_str_is_index_within_capacity(s, 2) == 1)
    ASSERT(le_str_is_index_within_capacity(s, 3) == 1)
    ASSERT(le_str_is_index_within_capacity(s, DEFAULT_CAPACITY) == 0)

    le_str_destroy(s);
}

void test_pointers() {
    struct le_str *s = le_str_create_with_cstr("Last chance to look at me, Hector");

    ASSERT(le_str_begin(s) == le_str_get_raw(s))
    ASSERT(le_str_end(s) == le_str_get_raw(s) + le_str_get_length(s))

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
    ASSERT(le_str_get_length(s) == 0);
    ASSERT(le_str_get_raw(s)[0] == 0);

    le_str_destroy(s);
}

void test_get_c() {
    char *cstr = "HOLY SHIT I'VE JUST HIT A CLIP";
    struct le_str *s = le_str_create_with_cstr(cstr);

    char c_from_method = le_str_get_at(s, 2);
    char c_from_data = le_str_get_raw(s)[2];
    char c_from_orig = cstr[2];
    ASSERT(c_from_method == c_from_orig)
    ASSERT(c_from_method == c_from_data)

    le_str_destroy(s);
}

void test_set_c() {
    char *cstr = "HOLY SHIT I'VE JUST HIT A CLIP";
    struct le_str *s = le_str_create_with_cstr(cstr);

    le_str_set_at(s, 7, '*');
    char c = le_str_get_at(s, 7);
    ASSERT(c == '*')

    le_str_destroy(s);
}

void test_dup() {
    struct le_str *s = le_str_create_with_cstr("Me when the");

    struct le_str *s_copy = le_str_dup(s);
    ASSERT(le_str_get_length(s_copy) == le_str_get_length(s))
    ASSERT(strcmp(le_str_get_raw(s_copy), le_str_get_raw(s)) == 0)
    ASSERT(le_str_is_valid(s_copy))

    le_str_destroy(s);
    le_str_destroy(s_copy);
}

void test_reverse() {
    struct le_str *s = le_str_create_with_cstr("Fortnite balls");

    struct le_str *s_reversed = le_str_get_reversed(s);
    size_t s_length = le_str_get_length(s);
    size_t s_reversed_length = le_str_get_length(s_reversed);

    ASSERT(s_reversed_length == s_length)

    for (size_t i = 0; i < le_str_get_length(s); i++) {
        ASSERT(le_str_get_at(s, i) == le_str_get_at(s_reversed, s_reversed_length - i - 1))
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

    ASSERT(strcmp(le_str_get_raw(slice1), "345") == 0)
    ASSERT(slice2 == (struct le_str *)-1)
    ASSERT(slice3 == (struct le_str *)-2)
    ASSERT(strcmp(le_str_get_raw(slice4), "9") == 0)

    le_str_destroy(s123456789);
    le_str_destroy(slice1);
    le_str_destroy(slice4);
}

void test_split() {
    struct le_str *basic = le_str_create_with_cstr(" tanki online  v2 ");
    struct le_str *space = le_str_create_with_cstr(" ");
    struct le_str **tokens = le_str_split(basic, space);
    struct le_str **token_ptr = tokens;

    size_t length = 0;

    ASSERT(strcmp(le_str_get_raw(tokens[0]), "") == 0)
    ASSERT(strcmp(le_str_get_raw(tokens[1]), "tanki") == 0)
    ASSERT(strcmp(le_str_get_raw(tokens[2]), "online") == 0)
    ASSERT(strcmp(le_str_get_raw(tokens[3]), "") == 0)
    ASSERT(strcmp(le_str_get_raw(tokens[4]), "v2") == 0)
    ASSERT(strcmp(le_str_get_raw(tokens[5]), "") == 0)

    // TODO: написать нормальный тест
    while (*token_ptr != NULL) {
        le_str_destroy((*token_ptr));
        token_ptr++;
        length++;
    }

    ASSERT(length == 6)

    le_str_destroy(basic);
    le_str_destroy(space);
    free(tokens);
}

void test_replace() {
    struct le_str *orig = le_str_create_with_cstr("I HATE JAVA!!  JAVA MAKES ME SICK. I HOPE ONE DAY JAVAA'LL BECOME OBSOLETE.");
    struct le_str *old = le_str_create_with_cstr("JAVA");
    struct le_str *new = le_str_create_with_cstr("C++");

    struct le_str *result = le_str_replace(orig, old, new);
    ASSERT(le_str_get_length(result) == 72)
    ASSERT(strcmp(le_str_get_raw(result), "I HATE C++!!  C++ MAKES ME SICK. I HOPE ONE DAY C++A'LL BECOME OBSOLETE.") == 0)

    le_str_destroy(orig);
    le_str_destroy(old);
    le_str_destroy(new);
    le_str_destroy(result);
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
    test_split();
    test_replace();

    printf("\nFinished. %d/%d tests passed.\n", assert_cntr - err_cntr, assert_cntr);
    return 0;
}
