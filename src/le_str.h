#ifndef LE_STR
#define LE_STR

#define DEFAULT_CHUNK_SIZE 32
#define TRUE 1
#define FALSE 0
typedef char BOOL;

struct le_str {
    size_t chunk_size;
    size_t length;
    char *data;
};

// le_str construction
struct le_str *le_str_create();

// le_str construction with cstr
struct le_str *le_str_create_with_cstr(char const *cstr);

// le_str construction with length
struct le_str *le_str_create_with_length(size_t length);

// le_str destruction
void le_str_destroy(struct le_str *s);

// Check if index is within a length of le_str
int le_str_is_index_within_length(struct le_str const *s, size_t index);

// Get le_str symbol on index
char le_str_get_c(struct le_str const *s, size_t index);

// Get le_str symbol on index unsafely
char __unsafe_le_str_get_c(struct le_str const *s, size_t index);

// Set le_str symbol on index
void le_str_set_c(struct le_str *s, size_t index, char c);

// Set le_str symbol on index unsafely
void __unsafe_le_str_set_c(struct le_str *s, size_t index, char c);

// Append cstr in the end of s data
void __le_str_append_cstr(struct le_str *s, char const *cstr);

// Append n symbols of cstr in the end of s data
void __le_str_append_ncstr(struct le_str *s, char const *cstr, size_t n);

// Get slice of le_str (a new le_str instance)
struct le_str *le_str_get_slice(struct le_str const *s, size_t start, size_t end);

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

#endif