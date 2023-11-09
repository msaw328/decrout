.PHONY: all dirs clear

.DEFAULT_GOAL := all

# C compiler flags
CFLAGS := -Wall -Werror -Wextra -pedantic -std=c99 -Isrc/

# List of source files
SRC := main.c io/fileread.c lexer/lexer.c lexer/token_list.c
SRC := $(patsubst %,src/%,$(SRC))

# List of object files of the library
OBJ := $(patsubst src/%.c,build/bin/%.o,$(SRC))

# 1 source file = 1 object file
build/bin/%.o: src/%.c
	$(CC) -c $(CFLAGS) $^ -o $@

# All object files = 1 library file
build/bin/comp: $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Phony targets below this point# build directory
dirs:
	mkdir -p $(dir $(OBJ))

all: dirs build/bin/comp

clear:
	rm -rf build
