#include <stdio.h>
#include <stdlib.h>

// Creating a hamming function that applies Hamming(7,4) error-correcting code to 4-bit data.
void hamming(unsigned char n, unsigned char *P1, unsigned char *P2, unsigned char *D1, unsigned char *P4, unsigned char *D2, unsigned char *D3, unsigned char *D4) {
	// Extracting individual data bits from the input.
	// Extracting the 1st data bit (most significant bit).
	// Extracting the 2nd and 3rd data bit.
	// Extracting the 4th data bit (least significant bit).
	*D1 = (n >> 3) & 1;
	*D2 = (n >> 2) & 1;
	*D3 = (n >> 1) & 1;
	*D4 = n & 1;
	
	// Calculating parity bits based on data bits using XOR.
        // Each parity bit is calculated as the XOR of three specific data bits.
	*P1 = *D1 ^ *D2 ^ *D4;
	*P2 = *D1 ^ *D3 ^ *D4;
	*P4 = *D2 ^ *D3 ^ *D4;
}

// Creating a nib function that processes each 4-bit nibble and calculates the Hamming(7,4) code.
void nib(unsigned char n, unsigned char *buff, int *index) {
	// Creating an array to hold the 7 bits resulting from the Hamming code (4 data bits and 3 parity bits).
	unsigned char bits[7];
	hamming(n, &bits[0], &bits[1], &bits[2], &bits[3], &bits[4], &bits[5], &bits[6]);
	
	// Looping through each of the 7 bits and shifting and combining them for final output.
	for(int x = 0; x < 7; x++) {
		buff[x] = (buff[x] << 1) | bits[x];
	}
	
	// Increment the index.
	(*index)++;
}

int main(int argc, char *argv[]) {
	// Checking if given command line args are valid.
	if (argc != 3 || strcmp(argv[1], "-f") != 0) {
		fprintf(stderr, "Usage: ./raid -f <filename>\n");
		exit(1);
	}
	
	// Opening the input binary file for reading.
	FILE *input = fopen(argv[2], "rb");
	if (!input) {
		perror("Can't open input file");
		exit(1);
	}

	// Creating and opening 7 separate output files.
        // Each file will store one bit position of the 7-bit Hamming code.
	char separateFile[100];
	FILE *sep[7];
	for (int x = 0; x < 7; x++) {
		sprintf(separateFile, "%s.part%d", argv[2], x);
		sep[x] = fopen(separateFile, "wb");

		if (!sep[x]) {
			perror("Can't open separate file");
			exit(1);
		}
	}

	// Creating a buffer to temporarily store the bits before they are written to the output files.
	// Creating the index to keep track of the number of nibbles processed.
	// Creating a Variable to hold bytes read from the input file.
	unsigned char buff[7] = {0};
	int index = 0;
	unsigned char byte;
	
	// Reading the input file byte by byte.
	// Separating the byte into two nibbles (4 bits each).
	// Processing each nibble to calculate the Hamming(7,4) code.
	// After processing 8 nibbles (a total of 56 bits), it then writes the buffer to the separate output files.
	// Finally, it resets the buffer after writing and the index.
	while (fread(&byte, 1, 1, input) == 1) {
		unsigned char leftnib = byte >> 4;
		unsigned char rightnib = byte & 0x0F;
		
		nib(leftnib, buff, &index);
		nib(rightnib, buff, &index);
		
		if (index == 8) {
			for (int x = 0; x < 7; x++) {
				fwrite(&buff[x], 1, 1, sep[x]);
				buff[x] = 0;
			}

			index = 0;
		}
	}
	
	// Simply closing all open files.
	fclose(input);
	for (int x = 0; x < 7; x++) {
		fclose(sep[x]);
	}

	return 0;
}

