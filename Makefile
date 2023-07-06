.PHONY: all clear

.DEFAULT_GOAL := all

# build directory
build:
	mkdir -p build/bin

# C compiler flags
CFLAGS := -Wall -Werror -Wextra -pedantic -std=c99

# List of source files
SRC := main.c fileread.c lexer.c types.c
SRC := $(patsubst %,src/%,$(SRC))

# List of object files of the library
OBJ := $(patsubst src/%.c,build/bin/%.o,$(SRC))

# 1 source file = 1 object file
build/bin/%.o: src/%.c
	$(CC) -c $(CFLAGS) $^ -o $@

# All object files = 1 library file
build/bin/comp: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Phony targets below this point
all: build build/bin/comp

clear:
	rm -rf build
