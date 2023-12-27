Name: Sri Sai Manjunadh Valluru (Sai)
Student ID: 01852662





A brief description of my project:
Raid.c the encoder:
This part of the project is dedicated to reading binary data and applying the Hamming(7, 4) encoding scheme to ensure error detection and correction. Here’s a brief overview for what I did for raid.c:

Input Reading: It reads binary data from a file, taking each byte and splitting it into two 4-bit nibbles.
Hamming Encoding: For each nibble, it calculates three parity bits and combines them with the original 4 data bits. This results in a 7-bit encoded data following the Hamming(7, 4) coding scheme.
File Writing: The encoded 7-bit data are written into seven separate files. Each file contains one bit of the 7-bit encoded data for each original nibble, which is distributing the data bits and parity bits across these files.
Error Handling: Appropriate error messages are displayed for issues like incorrect command-line arguments or problems with file operations, such as opening or closing.

Diar.c the (Decoder)
This part of the project is focused on taking the output from raid.c and decoding it, while also correcting errors if neccessary. Here’s a brief overview for what I did for diar.c:

File Reading: It reads from the seven separate files produced by raid.c. Each of them contains parts of the Hamming(7, 4) encoded data.
Error Correction and Decoding: For each 7-bit encoded data, it checks for errors using the parity bits. If a single-bit error is found, it corrects it. Then, it extracts the 4 data bits, decoding the Hamming(7, 4) code back to the original data.
Output: The decoded 4-bit data are combined into bytes and written into an output file, recreating the original binary data before the encoding process.
Error Handling: Like raid.c, it also includes error-handling mechanisms for various issues to make sure it is robust during the decoding process.
Overall, the project utilizes the Hamming(7, 4) code to create a simple 

RAID-like system where data is encoded, distributed across multiple files, and later retrieved by being decoded and error-corrected to reproduce the original data. For a more detailed description of the code, view the comments provided in the c files.





Commenting and explaining for each line of code in the given Makefile:
# Assigns the C compiler to be used. The 'cc' is typically an alias to the system’s default C compiler.
CC=cc

# Assigns compiler flags. The '-Wall' allows all compiler's warning messages, and '-O2' allows optimization.
CFLAGS=-Wall -O2

# This is a rule that states 'all' as the target. The 'all' is a conventional target to build.
# So all executables are raid and diar in this case.
all: raid diar

# This takes the .c file and compiles it into an executable. Creating raid.c to raid.exe or raid.
# The '$@' is the filename of the target while the '$<' is the filename of the first prerequisite.
%: %.c
        $(CC) $(CFLAGS) -o $@ $<

# This is a simple rule for the target 'clean'. Doesn't really have any prerequisites.
# It removes the listed files and allows for cleaning up the build environment, which is a good practice to have.
clean:
        rm -f a.out *.part? *.2





How getopt.h and my code reads input from the command line:
My code manually parses command-line arguments, directly accessing the argv array, instead of using the getopt.h. It checks the number of arguments and the correctness of flags, such as "-f" in raid.c and both "-f" and "-s" in diar.c. Filenames and other parameters are read from specific positions in the argv array, which then outputs error messages for unexpected formats or values.

If I were to use getopt.h, it would streamline argument parsing. Including <getopt.h>, options would be defined and getopt() will be called in a loop, returning each processed option character. A switch-case could handle each option easily in this scenario, making relevant actions or changes and overall simplifying the structure. After processing all options, the remaining arguments can be managed following the loop.
