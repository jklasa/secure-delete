#include <stdlib.h>

// Function pointer to overwrite method
typedef char (*overwriteType)(int, int);

// Struct defining the necessary variables related to a particular overwrite method
typedef struct Type {
	const char * const name;
	overwriteType overwrite;
} type_t;

// Array of possible overwrite methods to use
const extern type_t types[];

// Number of overwrite methods
const extern int NUM_TYPES;

// Descriptive output for usage
const extern char * const type_descriptions;

// Overwrite using zeros
char set_bytes(int pass, int byte);

// Overwrite using random bytes
char random_bytes(int pass, int byte);

// Attempt at using the Gutmann method of data erasure byte by byte
char mixed_bytes(int pass, int byte);
