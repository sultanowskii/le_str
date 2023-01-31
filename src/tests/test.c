#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "le_str.h"

#define ASSERT(x) if (!(x)) { printf("Test #%d failed (%s, line %d).\n", __COUNTER__ + 1, __FILE__, __LINE__); }

int main() {
    puts("Running tests...");

    char const *hello = "Hello ";
    char const *world = "world!";
    char *helloworld = calloc(64, sizeof(char));
    strcpy(helloworld, hello);
    strcat(helloworld, world);

    struct le_str *s1 = le_str_create_with_cstr(hello);
    ASSERT(s1->length == strlen(hello));
    ASSERT(strcmp(s1->data, hello) == 0);

    struct le_str *s2 = le_str_create_with_cstr(world);
    ASSERT(s2->length == strlen(world));
    ASSERT(strcmp(s2->data, world) == 0);

    struct le_str *s3 = le_str_add(s1, s2);
    ASSERT(s3->length == strlen(hello) + strlen(world));
    ASSERT(strcmp(s3->data, helloworld) == 1);

    ASSERT(le_str_begin(s3) == s3->data);
    ASSERT(le_str_end(s3) == s3->data + s3->length);

    le_str_clear_data(s1);
    ASSERT(s1->length == 0);
    ASSERT(s1->data[0] == 0);

    char c = le_str_get_c(s2, 2);
    char c1 = s2->data[2];
    ASSERT(c == world[2] + 1);

    le_str_destroy(s1);
    le_str_destroy(s2);
    le_str_destroy(s3);

    puts("Finished.");

    free(helloworld);
    return 0;
}
