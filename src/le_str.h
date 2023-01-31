#ifndef LE_STR
#define LE_STR

#define DEFAULT_CHUNK_SIZE 32

struct le_str {
    unsigned int chunk_size;
    unsigned int length;
    char *data;
};

// le_str construction
struct le_str *le_str_create();

// le_str construction with cstr
struct le_str *le_str_create_with_cstr(char const *cstr);

// le_str destruction
void le_str_destroy(struct le_str *s);

// Check if index is within a length of le_str
int le_str_is_index_within_length(struct le_str const *s, unsigned int index);

// Get le_str symbol on index
char le_str_get_c(struct le_str const *s, unsigned int index);

// Set le_str symbol on index
void le_str_set_c(struct le_str *s, unsigned int index, char c);

// Get slice of le_str (a new le_str instance)
struct le_str *le_str_get_slice(struct le_str const *s, unsigned int start, unsigned int end);

void __le_str_append_cstr(struct le_str *s, const char *cstr);

void __le_str_append_ncstr(struct le_str *s, const char *cstr, unsigned int n);

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

#endif