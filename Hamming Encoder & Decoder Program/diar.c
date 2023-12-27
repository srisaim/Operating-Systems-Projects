#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Creating a function to decode. 7-bit Hamming code -> 4-bit data
// The function takes a 7-bit array as input and outputs the decoded 4-bit data.
unsigned char decode(unsigned char bits[7]) {
	unsigned char D1, D2, D3, D4;
	unsigned char checkP1, checkP2, checkP4;
	unsigned char epos;
	
	// Extracting data bits from the input array.
	D1 = bits[2];
	D2 = bits[4];
	D3 = bits[5];
	D4 = bits[6];
	
	// Checking and verifying the parity bits to find the position of an error, if there is one.
	checkP1 = D1 ^ D2 ^ D4;
	checkP2 = D1 ^ D3 ^ D4;
	checkP4 = D2 ^ D3 ^ D4;
	
	// Calculating the error position using the calculated parity bits.
	epos = checkP1 * 1 + checkP2 * 2 + checkP4 * 4;
	
	// If an error is found, correct it by flipping the faulty bit.
	if (0 < epos) {
		bits[epos - 1] ^= 1;  // correct the error
	}

	// Simply return the corrected 4-bit data.
	return (D1 << 3) | (D2 << 2) | (D3 << 1) | D4;
}

int main(int argc, char *argv[]) {
	// Checking if given command line args are valid.
	if (argc != 5 || strcmp(argv[1], "-f") != 0 || strcmp(argv[3], "-s") != 0) {
		fprintf(stderr, "Usage: ./diar -f <filename> -s <numberOfBytes>\n");
		exit(1);
	}
	
	// Opening the 7 separate files that contain parts of the Hamming code.
	FILE *sep[7];
	for (int x = 0; x < 7; x++) {
		char separateFile[100];
		sprintf(separateFile, "%s.part%d", argv[2], x);
		sep[x] = fopen(separateFile, "rb");

		if (!sep[x]) {
			perror("Can't open separate file");
			exit(1);
		}
	}
	
	// Opening the output file where the decoded data will be written.
	FILE *output = fopen(strcat(argv[2], ".2"), "wb");
	
	if (!output) {
		perror("Can't open output file");
		exit(1);
	}
	
	// Buffer to temporarily hold the bits read from the separate files.
	unsigned char buff[7];
	unsigned char nib[2];
	int numBytes = atoi(argv[4]);
	
	// Reading bits from the separate files.
	for(int y = 0; y < numBytes; y++) {
		int bytesRead = 0;
		// Reading bits from the separate files to the buffer.
		for (int x = 0; x < 7; x++) {
			bytesRead += fread(&buff[x], sizeof(unsigned char), 1, sep[x]);
		}
		
		if (bytesRead < 7) {
			break;
		}
		
		// Decoding the bits read from the buffer.
		for (int z = 0; z < 8; z++) {
			unsigned char bits[7];

			// Extracting bits for decoding.
			for (int x = 0; x < 7; x++) {
				bits[x] = (buff[x] >> (7 - z)) & 1;
			}
			
			// Decoding the bits and storing them temporarily in the nib array.
			nib[z % 2] = decode(bits);

			// If two nibbles have been decoded, then combine them into a byte and write to the output file.
			if (z % 2) {
				unsigned char byte = (nib[0] << 4) | nib[1];
				fwrite(&byte, sizeof(unsigned char), 1, output);
			}
		}
	}
	
	// Simply Closing all open files.
	for (int x = 0; x < 7; x++) {
		fclose(sep[x]);
	}

	fclose(output);
	
	return 0;
}

