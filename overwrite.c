#include <stdio.h>
#include <stdlib.h>
#include "overwrite.h"

/*
 * Only this file (and the header file) needs to be edited when adding a new overwrite method
 */


// Available overwrite types
const Type types[] = { {.name = "Set Bytes", .overwrite = &set_bytes},
						{.name = "Random", .overwrite = &random_bytes},
						{.name = "Gutmann", .overwrite = &mixed_bytes} };
const int NUM_TYPES = 3;
const char * const type_descriptions = "Overwrite types: (1) set bytes (0s), (2) random bytes, (3) mixed (using Gutmann's method)";

// Overwrite using zeros
char set_bytes(int pass, int byte) {
	return byte;
}

// Overwrite using random bytes
char random_bytes(int pass, int curr_byte) {
	return rand() % 256;
}

// Attempt at using the Gutmann method of data erasure byte by byte
char mixed_bytes(int pass, int curr_byte) {
	char val;
	int b;

	switch (pass) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 32:
		case 33:
		case 34:
		case 35:
			val = rand() % 256; break;
		case 5:
			val = 'U'; break;
		case 6:
			val = 170; break;
		case 26:
		case 7:
			b = curr_byte % 3;
			switch (b) {
				case 0:
					val = 146; break;
				case 1:
					val = 73; break;
				case 2:
					val = 36; break;
			}
			break;
		case 27:
		case 8:
			b = curr_byte % 3;
			switch (b) {
				case 0:
					val = 73; break;
				case 1:
					val = 36; break;
				case 2:
					val = 146; break;
			}
			break;
		case 28:
		case 9:
			b = curr_byte % 3;
			switch (b) {
				case 0:
					val = 36; break;
				case 1:
					val = 146; break;
				case 2:
					val = 73; break;
			}
			break;
		case 10:
			val = 0; break;
		case 11:
			val = 17; break;
		case 12:
			val = 34; break;
		case 13:
			val = 51; break;
		case 14:
			val = 68; break;
		case 15:
			val = 85; break;
		case 16:
			val = 102; break;
		case 17:
			val = 119; break;
		case 18:
			val = 136; break;
		case 19:
			val = 153; break;
		case 20:
			val = 170; break;
		case 21:
			val = 187; break;
		case 22:
			val = 204; break;
		case 23:
			val = 221; break;
		case 24:
			val = 238; break;
		case 25:
			val = 255; break;
		case 29:
			b = curr_byte % 3;
			switch (b) {
				case 0:
					val = 109; break;
				case 1:
					val = 182; break;
				case 2:
					val = 219; break;
			}
			break;
		case 30:
			b = curr_byte % 3;
			switch (b) {
				case 0:
					val = 182; break;
				case 1:
					val = 219; break;
				case 2:
					val = 109; break;
			}
			break;
		case 31:
			b = curr_byte % 3;
			switch (b) {
				case 0:
					val = 216; break;
				case 1:
					val = 109; break;
				case 2:
					val = 182; break;
			}
			break;
		default:
			/*
			// Abort this method if an invalid pass is provided (this results in a messy exit - no cleanup is done)
			puts("--Invalid pass using Gutmann's method");
			exit(1); 
			*/

			val = rand() % 256; break;
	   }
	
	return val;
}
