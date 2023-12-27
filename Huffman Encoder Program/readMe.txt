Name: Sri Sai Manjunadh Valluru

Approach for proj1:
Before starting the project, I did my basic research and understanding of Huffman coding. I took my time to understand the overall algorithms of Huffman coding, and why its used for its benefits. For my research, I read some text, and online forums/articles, and watched some small tutorial videos on YouTube. After researching, I started to plan by creating pseudocode for the entire project.  This allowed me to have a clear view of what aspects need to be worked on, like what data structures to be used and allowed the program to be broken down. I started small by reading the file and calculating the frequency of each character, then implemented the priority queue and tested it separately to make sure it worked. Which then led to constructing the Huffman tree and visualizing it, while building the Huffman codes for each character and testing its functionality. When coding I also didn't code everything at once but rather implemented a functionality one at a time before moving to the next part. I also check for any errors when dealing with file operations. This allows the program to be robust and can handle edge cases if needed. A big part in helping me stay on track and code was commenting along the way. I commented as I coded in a minimal manner, and at the end of the test phasing added extra comments to make a better understanding. Unfortunately, I wasn't able to successfully finish the program as intended, as differences were found by diff mytmp.out tmp.out, but I tried patiently to fix it, but just ended up running out of time.

Purpose of proj1:
The main purpose and functionality of the Huffman coding code is to read an input file, completeShakespeare.txt, calculate the frequency of each byte, or character, in the text file, build a Huffman tree based on the frequencies, encode the file data using the Huffman codes gotten from the tree, and lastly, write the compressed data to an output file, myhuffman.out.

This creates a node structure that will be used to build the Huffman tree. The nodes can either represent an internal node or a leaf.
The other global variables like node pool pool, priority queue qqq, and codes buffer buf, are defined to help in the Huffman tree construction and encoding.
typedef struct node_t {
     struct node_t *right;
     struct node_t *left;
     int freq;
     unsigned char c;
}    *node;


The priority queue used as an array, helps construct the Huffman tree. Nodes are inserted depending on their frequency, where the least frequent nodes are at the front. The functions qinsert and qremove help manage this queue. Nodes with character frequencies are at first added to the priority queue. The tree is constructed like this:

Continuously taking out the two nodes with the lowest frequencies, using qremove.
Creating a new parent node for them with a summed frequency.
Inserting this new node back into the priority queue using qinsert.

This process repeats until only one node is left at the root of the Huffman tree. When it comes to generating the Huffman code, the build_code function is a recursive function that traverses through the Huffman tree. It creates binary strings for each character based on the path for it to reach that character from the root. When moving, for left, it adds "1", and for right, it adds "0", to the code. In the encoding data part, the program reads the input file again, and for each character, gets its Huffman code and writes the code in binary to the output file. The binary data is managed using buffer, which collects bits until a full byte is ready to be written. Lastly, in command line argument parsing, the program simply accepts optional command line arguments for specifying input and output filenames using -i and -o.

When it comes to special features, I'd say this code has:
Memory management, because instead of dynamic memory allocation, this code uses a static pool for nodes. I found this an efficient approach because the max number of possible unique characters in the file is 256.
Bitwise Operations, because when encoding, the program uses bitwise operations for efficient buffer management. This allows the compressed data to be packed bit by bit.
Error handling, as the code checks for potential file opening errors, makes sure that the program doesn't crash if the provided filenames are incorrect.
