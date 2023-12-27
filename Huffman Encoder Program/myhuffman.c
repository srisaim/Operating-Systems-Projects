// Including  necessary headers for functions.  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
                                                
// Creating pointers to right and left child nodes.
// Creating int freq for frequency of characters
// Creating unsigned char c for the character itself.
typedef struct node_t {
	struct node_t *right;
	struct node_t *left;
	int freq;
	unsigned char c;
} 
// Create a new data type, node, which is a pointer to struct node_t.
*node;

// Creating new data type, node, which is pointer to struct node_t.
// Creating int n_nodes to track number of nodes.
// Creating int qend for end index of priority queue.
// Pool of nodes where the total number of ASCII characters is 256.
// All queue of nodes, where pointer q is initialized to one element before array start.
// Code is array of pointers to Huffman codes & buffer is to hold Huffman codes.
int n_nodes = 0;
int qend = 1;
struct node_t pool[256] = {{0}};
node qqq[255], *q = qqq - 1;
char *code[256] = {0}, buf[4096];

// Creating a new node constructor, the function new_node initializes node from the pool 
// with the provided frequency, character, and right left children.
// Get node from pool and increment the global node counter.
// If freq is provided, it's a leaf node, else it’s an internal node.
// Initializes the character and frequency if it's a leaf node.
// Else, intializes children and frequency as sum of children's frequencies.
// Simply returns the initialized node.
node new_node(int freq, unsigned char c, node a, node b) {
	node n = pool + n_nodes++;
	if (freq) {
		n->c = c;
		n->freq = freq;
	}
	else {
		n->left = a;
		n->right = b;
		n->freq = a->freq + b->freq;
	}
	return n;
}

// Inserting node n into priority queue q.
// Start from the end of queue & increment the end index.
// Using while-loop, loop until the parent’s frequency is larger than the inserted node.
// Using if-statement, break if the parent’s frequency is smaller than or equal to the inserted node’s frequency.
// Else, move the parent down and continue.
// Lastly, insert the node in the found position.
void qinsert(node n) {
	int j;
	int i = qend++;
	while ((j = i / 2)) { 
		if (q[j]->freq <= n->freq) {
			break;
		}
		q[i] = q[j];
		i = j;
	}
	q[i] = n;
}

// Removing & returning the node with the smallest frequency from the priority queue.
// Starting from the root of the heap, using if-statement, check if there’s no node in the queue and return null.
// Decrement qend, the end index.
// Using while-loop, find the child with the smaller frequency to replace removed root.
// If it has smaller frequency, choose the right child, move the child up, and continue.
// Place the last node in the queue in the found position, and simply return the removed node. 
node qremove() {
	int i;
	int l;
	node n = q[i = 1];
	if (qend < 2) {
		return 0;
	}
	qend--;
	while ((l = i * 2) < qend) {
		if (l + 1 < qend && q[l + 1]->freq < q[l]->freq) {
			l++;
		}
		q[i] = q[l], i = l;
	}
	q[i] = q[qend];
	return n;
}

// This is to recursively build Huffman codes using the Huffman tree.
// Creating static pointer to the buffer, which stores the Huffman codes.
// Using if-statement, check if it's a leaf node, if it is, then it contains a character.
// Set code string to 0, copy the code string to the buffer, assign the buffer pointer to the code of the character,
// move the buffer pointer, and simply return because it's a leaf node.
// For the left child and recurse, assign "1".
// For the right child and recurse, assign "0".  
void build_code(node n, char *s, int len) {
	static char *out = buf;
	if (n->c) {
		s[len] = 0;
		strcpy(out, s);
		code[n->c] = out;
		out += len + 1;
		return;
	}
	s[len] = '1'; 
	build_code(n->left,  s, len + 1);
	s[len] = '0'; 
	build_code(n->right, s, len + 1);
}

// The main function for the program.
int main(int argc, char *argv[]) {
	// Initializing the filenames for input and output.
	// Creating an array to store frequency of characters.
	// Creating file pointers for input and output files.
	// Creating variable char c to read characters from input file.
	// Creating string to store Huffman codes when traversing tree.
	// Creating buffer to write the compressed data.
	// Creating counter for number of bits in the buffer, because buffer writes byte by byte.
	char input_filename[255] = "completeShakespeare.txt";
	char output_filename[255] = "myhuffman.out";
	unsigned int freq[256] = {0};
	FILE *fp_in;
	FILE *fp_out;
	unsigned char c;
	char s[16] = {0};
	unsigned char buffer = 0;
	int bits_in_buffer = 0;
	
	// Using for-loop, creating parse command line argument for input and output files.
	// Using if-statement, check if the argument is "-i", if it is, it’s an input filename.
	// Using if-statement, check if there's a value given, if there is, copy value to input_filename.
	// Else-if, check if the argument is "-o", if it is, it's an output filename.
	// Using if-statement, check if there's a value given, if there is, copy value to output_filename.
	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-i") == 0) {
			if (++i < argc) { 
				strcpy(input_filename, argv[i]);
			}
		}
		else if (strcmp(argv[i], "-o") == 0) {
			if (++i < argc) strcpy(output_filename, argv[i]);
		}
	}
	
	// Using if-statement, it will try to open the input file.
	// An error message is printed if input file couldn't open, and simply returns 1.
	if (!(fp_in = fopen(input_filename, "rb"))) {
		perror("Input file failed to open");
		return 1;
	}
	
	// Using while-loop, it will read characters from the input file and calculate their frequencies.
	while (fread(&c, 1, 1, fp_in) == 1) {
		freq[c]++;
	}

	// Using for-loop, it will insert nodes with characters and frequencies into the priority queue.
	for (int i = 0; i < 256; i++) {
		if (freq[i]) {
			qinsert(new_node(freq[i], i, 0, 0));
		}
	}
	
	// Using while-loop, it will build the Huffman tree from the priority queue.
	// Store the first qremove in a temp variable, and pass it as second argument, to make sure the node with the least frequency is on the right.
	while (qend > 2) {
		node least_freq_node = qremove();
		qinsert(new_node(0, 0, qremove(), least_freq_node));
	}
	
	// Simply used to build Huffman codes from the Huffman tree.
	build_code(q[1], s, 0);
	
	// Using if-statement, it will try to open the output file.
        // An error message is printed if output file couldn't open, and simply returns 1.
	if (!(fp_out = fopen(output_filename, "wb"))) {
		perror("Output file failed to open");
		return 1;
	}
	

	// Re-reads the input file and writes the compressed data to the output file.
	// File pointer goes to beginning of the input file.
	rewind(fp_in);
	// Using while-loop, reads one character at a time from the input file until the end.
	// Gets the Huffman code of the character.
	// Using while-loop, iterates over the Huffman code each character.
	// Shifts the buffer one bit left, which makes room for next bit.
	// Using if-statment, check if the current Huffman code bit is "1", if it is, set the least significant bit of the buffer.
	// Increment counter for bits in the buffer.
	// Using if-statement, check if the buffer is full, or contains 8 bits, if it does, write the byte to the output file.
	// Reset the buffer and the bits counter for next byte.
	while (fread(&c, 1, 1, fp_in) == 1) {
		char *s = code[c];
		while (*s) {
			buffer <<= 1;
			if (*s++ == '1') {
				buffer |= 1;
			}
			if (++bits_in_buffer == 8) {
				fwrite(&buffer, 1, 1, fp_out);
				buffer = 0;
				bits_in_buffer = 0;
			}
		}
	}

	// Using if-statement, if there remains any bits in the buffer, write them to file.
	// Pad the remaining bits in buffer with zeros to the right.
	// Write last byte to output file.
	if (bits_in_buffer) {
		buffer <<= (8 - bits_in_buffer);
		fwrite(&buffer, 1, 1, fp_out);
	}

	// Simply closes the input and output files.
	fclose(fp_in);
	fclose(fp_out);
	
	return 0;
}
