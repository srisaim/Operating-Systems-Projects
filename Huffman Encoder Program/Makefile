# Creating a simple Makefile for project.

# Compiler is used, when "gcc" is used.
CC = gcc

# Compiler Flags to catch issues in code when compiling.
# -Wall is general warning messages about constructions, which some users miss.
# -Wextra is extra warning flags that are not enabled by Wall, like uninitialized variables.
# -pedantic is usually stricter warnings which C follows, like using  non-standard language extensions.
# -std=c99 tells which version of the C lang standard to be used. This makes sure that the compiler will follow the specified standard.
CFLAGS = -Wall -Wextra -pedantic -std=c99

# Tells what to name the executable when created.
EXEC = myhuffman

# Tells which source or ".c" files to compile.
SRC = myhuffman.c

# Tells which object or ".o" files to be created from the source files.
OBJ = $(SRC:.c=.o)

# How the executable is being built.
# $(EXEC) is used to link the object files and create the executable.
# %.o: %.c is used to compile the source files into object files.
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the built objects and executable, or basically by remove the built objects files and executable.
clean:
	rm -f $(OBJ) $(EXEC)

