#include <time.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include "overwrite.h"

int main(int argc, char **argv) {
	if (argc <= 1) {
		puts("Correct usage: sdel <filename> [overwrite type (default=1) [number of passes (default=7, or 35 if type==3)]]");
		puts("\tOverwrite types: (1) set bytes (0s), (2) random bytes, (3) mixed (using Gutmann's method)");
		exit(1);
	}

	// Get the file descriptor
	int fd = open(argv[1], O_RDWR, S_IWRITE | S_IREAD);
    if (fd < 0) {
        puts("--Invalid file");
        exit(1);
    }

	// Get size of the file
	int size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);

	// Set overwrite type
	int type = 1;
    long nPasses = 7;
	if (argc >= 3) {
		char *ptr;
		type = strtol(argv[2], &ptr, 10);

		if (*ptr != '\0') {
			puts("--Error parsing type");
			close(fd);
			exit(1);
		}
	}

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
            nPasses = 35;
			break;
		default:
			puts("--Invalid overwrite type");
			close(fd);
			exit(1);
	}

	// Set number of passes
	if (argc == 4) {
		char *ptr;
		nPasses = strtol(argv[3], &ptr, 10);

		if (*ptr != '\0') {
			puts("--Error parsing number");
			close(fd);
			exit(1);
		}

		if (nPasses < 0) {
			puts("--Invalid number of passes");
			close(fd);
			exit(1);
		}
	}

	const char * const types[] = { "Zero", "Random", "Gutmann" };
	printf("Deleting '%s' with %ld passes of %s method...\n", argv[1], nPasses, types[type - 1]);

	// Create buffer to write
	char *str = malloc(1 * sizeof *str);
	if (str == NULL) {
		puts("--Error creating buffer to write");
		exit(1);
	}

	// Overwrite all bytes in the file
	int i, j;
	for (i = 1; i <= nPasses; i++) {
		for (j = 0; j < size; j++) {
			// Flush the bytes from the kerrnel buffer to the disk one byte at a time
			str[0] = overwrite(i, j);
			write(fd, str, 1);
			fsync(fd);		
		}
		lseek(fd, 0, SEEK_SET);
	}

	puts("Done");

	// Actually remove the file
	remove(argv[1]);

	// Cleanup and exit
	free(str);
	close(fd);
	return 0;	
}