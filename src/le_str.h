#ifndef LE_STR
#define LE_STR

#define DEFAULT_CAPACITY 32
#define TRUE 1
#define FALSE 0
typedef char BOOL;

struct le_str;

// le_str construction
struct le_str *le_str_create();

// le_str construction with cstr
struct le_str *le_str_create_with_cstr(char const *cstr);

// le_str construction with length
struct le_str *le_str_create_with_length(size_t length);

// le_str destruction
void le_str_destroy(struct le_str *s);

// Check if index is within a length of le_str
int le_str_is_index_within_capacity(struct le_str const *s, size_t index);

// Get le_str symbol on index
char le_str_get_at(struct le_str const *s, size_t index);

// Set le_str symbol on index
void le_str_set_at(struct le_str *s, size_t index, char c);

// Create a concatenation of two le_strs (new instance)
struct le_str *le_str_add(struct le_str const *first, struct le_str const *second);

// Add second le_str to the first one (same instance, second is not affected)
void le_str_concat(struct le_str *s, struct le_str const *second);

// Data begin pointer
char *le_str_begin(struct le_str const *s);

// Data end pointer
char *le_str_end(struct le_str const *s);

// Clear data (allocated chunk is not affected)
void le_str_clear_data(struct le_str *s);

// Create a copy of le_str. Please note it creates a new instance, only copying data.
// Therefore, some field values (such as `chunk_size`) may not be the same after dupping.
struct le_str *le_str_dup(struct le_str const *s);

// Creates a copy of le_str with reversed data.
struct le_str *le_str_get_reversed(struct le_str const *s);

// Validates fields integrity. If everythinh is ok, returns TRUE (1)
BOOL le_str_is_valid(struct le_str const *s);

// Get index of given char (`c`) in `s`. If not found, returns -1.
size_t le_str_index(struct le_str const *s, char c);

// Get rindex (index, starting from the end of string) of given char (`c`) in `s`. If not found, returns -1.
size_t le_str_rindex(struct le_str const *s, char c);

// Find index of substring (`sub`) in `s`.
size_t le_str_find(struct le_str const *s, struct le_str const *sub);

// Find index of substring (`sub`) in `s` with offset `n`. That means it will return index of `n`th match. 
size_t le_str_find_n(struct le_str const *s, struct le_str const *sub, unsigned int n);

// Get slice of string (both bounds are inclusive).
// Returns -1 if `start` or `end` are not within [0; length-1]
// Returns -2 if `start` > `end`
struct le_str *le_str_slice(struct le_str const *s, size_t start, size_t end);

// Split string using delimiter (returns array of le_strs). Don't forget to destroy it too!
struct le_str **le_str_split(struct le_str const *s, struct le_str const *delimiter);

// Replace all occurrences of `old` to `new` in s (new instance).
struct le_str *le_str_replace(struct le_str const *s, struct le_str const *old, struct le_str const *new);

// Get raw data
const char *le_str_get_raw(struct le_str const *s);

// Get capaciry
size_t le_str_get_capacity(struct le_str const *s);

// Get length
size_t le_str_get_length(struct le_str const *s);

#endif