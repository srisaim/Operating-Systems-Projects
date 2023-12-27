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
