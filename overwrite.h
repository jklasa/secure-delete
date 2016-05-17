typedef char (*overwriteType)(int, int);

// Overwrite using zeros
char set_bytes(int pass, int byte);

// Overwrite using random bytes
char random_bytes(int pass, int byte);

// Attempt at using the Gutmann method of data erasure byte by byte
char mixed_bytes(int pass, int byte);
