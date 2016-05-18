#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "overwrite.h"

// Quick bool definition
typedef int bool;
enum { false, true };

// Parse string input to an integer value
bool str_to_int(char *str, int *val) {
	if (val == NULL) {
		return false;
	}

	char *ptr;
	*val = strtol(str, &ptr, 10);

	if (*ptr != '\0') {
		printf("--Error parsing value '%s'\n", str);
		return false;
	}

	return true;
}

int main(int argc, char **argv) {
	if (argc <= 1) {
		puts("Correct usage: ./sdel <filename>");
		puts("\t[-t overwrite type (default=1)]");
		puts("\t[-n number of passes (default=7, or 35 if type==3)]");
		puts("\t[-s maximum buffer size between disk writes (default=1)]");
		puts("\t[-b set byte (decimal value) to overwrite in type 1 (default=0)]");
		puts("Overwrite types: (1) set bytes (0s), (2) random bytes, (3) mixed (using Gutmann's method)");
		exit(1);
	}

	// Get the file descriptor
	int fd = open(argv[1], O_RDWR, S_IWRITE | S_IREAD);
	if (fd < 0) {
	    puts("--Invalid file");
	    exit(1);
	}

	// Define program settings
	int type = 1;
	int num_passes = 7;
	int buf_size = 1;
	int byte = 0;

	// Read all inputs into the program
	bool failure = false;
	int a;
	for (a = 2; a < argc; a += 2) {
		if (!strcmp(argv[a], "-t")) {
			if (!str_to_int(argv[a + 1], &type)) {
				failure = true;
				break;
			}

			if (type == 3) {
				num_passes = 35;
			}
		} else if (!strcmp(argv[a], "-n")) {
			if (!str_to_int(argv[a + 1], &num_passes)) {
				failure = true;
				break;
			}

			if (num_passes < 0) {
				puts("--Invalid number of passes");
				failure = true;
				break;
			}
		} else if (!strcmp(argv[a], "-s")) {
			if (!str_to_int(argv[a + 1], &buf_size)) {
				failure = true;
				break;
			}

			if (buf_size < 1) {
				puts("--Invalid buffer size");
				failure = true;
				break;
			}
		} else if (!strcmp(argv[a], "-b")) {
			if (!str_to_int(argv[a + 1], &byte)) {
				failure = true;
				break;
			}

			if (byte < 0 || byte > 255) {
				puts("--Invalid byte size");
				failure = true;
				break;
			}
		}
	}

	// Cleanup and exit in case of failure while reading inputs
	if (failure) {
		close(fd);
		exit(1);
	}

	// Get size of the file
	int size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	// Set overwrite function based on type
	overwriteType overwrite;
	switch (type) {
		case 1:
			overwrite = &set_bytes;
			break;
		case 2:
			srand(time(NULL));
			overwrite = &random_bytes;
			break;
		case 3:
			srand(time(NULL));
			overwrite = &mixed_bytes;
			break;
		default:
			puts("--Invalid overwrite type");
			close(fd);
			exit(1);
	}

	// Print out the deletion settings
	const char * const types[] = { "Set Byte", "Random", "Gutmann" };
	printf("Deleting file '%s' with %d passes of %s method with byte %d and buffer size %d...\n", argv[1], num_passes, types[type - 1], byte, buf_size);

	// Create buffer to write
	char *buf = malloc(buf_size * sizeof *buf);
	if (buf == NULL) {
		puts("--Error creating char buffer");
		exit(1);
	}

	// Overwrite all bytes in the file
	int i, j, k;
	for (i = 1; i <= num_passes; i++) {
		for (j = 0; j < size; j += buf_size) {
			// Write and flush the bytes from the kerrnel buffer to the disk for each buffer of length buf_size
			for (k = 0; k < buf_size; k++) {
				if (type == 3) {
					buf[k] = overwrite(i, j + k);
				} else {
					buf[k] = overwrite(i, byte);
				}
			}
			write(fd, buf, buf_size);
			fsync(fd);
		}
		lseek(fd, 0, SEEK_SET);
	}

	puts("Done");

	// Actually remove the file
	remove(argv[1]);

	// Cleanup and exit
	free(buf);
	close(fd);
	return 0;	
}
