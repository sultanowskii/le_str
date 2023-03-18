#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "le_str.h"

#define ASSERT(x) if (!(x)) { printf("Test #%d failed (%s, line %d).\n", __COUNTER__ + 1, __FILE__, __LINE__); }

int main() {
    puts("Running tests...");

    struct le_str *empty = le_str_create_with_cstr("");

    char const *hello = "Hello ";
    char const *world = "world!";
    char *helloworld = calloc(64, sizeof(char));
    strcpy(helloworld, hello);
    strcat(helloworld, world);

    struct le_str *s1 = le_str_create_with_cstr(hello);
    ASSERT(s1->length == strlen(hello))
    ASSERT(strcmp(s1->data, hello) == 0)
    ASSERT(le_str_is_valid(s1))

    ASSERT(le_str_index(s1, 'l') == 2)
    ASSERT(le_str_rindex(s1, 'l') == 3)
    ASSERT(le_str_index(s1, 'Q') == (size_t)-1)
    ASSERT(le_str_rindex(s1, 'Q') == (size_t)-1)

    struct le_str *s2 = le_str_create_with_cstr(world);
    ASSERT(s2->length == strlen(world))
    ASSERT(strcmp(s2->data, world) == 0)
    ASSERT(le_str_is_valid(s2))

    struct le_str *s3 = le_str_add(s1, s2);
    ASSERT(s3->length == strlen(hello) + strlen(world))
    ASSERT(strcmp(s3->data, helloworld) == 0)
    ASSERT(le_str_is_valid(s3))

    ASSERT(le_str_begin(s3) == s3->data)
    ASSERT(le_str_end(s3) == s3->data + s3->length)

    le_str_clear_data(s1);
    ASSERT(s1->length == 0);
    ASSERT(s1->data[0] == 0);

    char c = le_str_get_c(s2, 2);
    char c1 = s2->data[2];
    ASSERT(c == world[2])
    ASSERT(c == c1)

    struct le_str *s3_copy = le_str_dup(s3);
    ASSERT(s3_copy->length == s3->length)
    ASSERT(strcmp(s3_copy->data, s3->data) == 0)
    ASSERT(le_str_is_valid(s3_copy))

    struct le_str *s3_reversed = le_str_get_reversed(s3);
    ASSERT(s3_copy->length == s3->length)
    for (size_t i = 0; i < s3->length; i++) {
        ASSERT(le_str_get_c(s3, i) == le_str_get_c(s3_reversed, s3_reversed->length - i - 1))
    }
    ASSERT(le_str_is_valid(s3_reversed))

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

    struct le_str *s123456789 = le_str_create_with_cstr("123456789");
    struct le_str *sliced1 = le_str_slice(s123456789, 2, 4);
    struct le_str *sliced2 = le_str_slice(s123456789, 2, 100);
    struct le_str *sliced3 = le_str_slice(s123456789, 5, 4);
    struct le_str *sliced4 = le_str_slice(s123456789, 8, 8);
    ASSERT(strcmp(sliced1->data, "345") == 0)
    ASSERT(sliced2 == (struct le_str *)-1)
    ASSERT(sliced3 == (struct le_str *)-2)
    ASSERT(strcmp(sliced4->data, "9") == 0)
    le_str_destroy(s123456789);
    le_str_destroy(sliced1);
    le_str_destroy(sliced4);

    puts("Finished.");

    free(helloworld);
    return 0;
}
