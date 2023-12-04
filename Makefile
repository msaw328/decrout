.PHONY: all dirs clear

PROJECT_NAME := dcrtc

COMMIT_ID := $(shell git rev-parse HEAD)
BUILD_DATE := $(shell date --rfc-3339=seconds)

.DEFAULT_GOAL := all

# C compiler flags
# One can define more flags using make MORE_FLAGS="..." all
CFLAGS := -DDCRTC_COMMIT_ID="\"$(COMMIT_ID)\"" -DDCRTC_BUILD_DATE="\"$(BUILD_DATE)\"" $(MORE_FLAGS) -Wall -Werror -Wextra -pedantic -std=c99 -Isrc/

# List of source files
SRC := 	main.c \
		context/args.c \
		io/fileread.c \
		lexer/lexer.c lexer/token_list.c lexer/output.c \
		types/types.c types/type_list.c \
		ast/ast.c ast/decl_list.c \
		parser/parser.c parser/parse_types.c parser/output.c

SRC := $(patsubst %,src/%,$(SRC))

# List of object files of the library
OBJ := $(patsubst src/%.c,build/obj/%.o,$(SRC))

# 1 source file = 1 object file
build/obj/%.o: src/%.c
	@$(CC) -c $(CFLAGS) $^ -o $@
	@echo -e "\t[CC] $@ <- $^"

# All object files = 1 library file
build/$(PROJECT_NAME): $(OBJ)
	@$(CC) $(CFLAGS) $^ -o $@
	@echo -e "\t[LD] $@ < $^"

# Phony targets below this point# build directory
dirs:
	@mkdir -p $(dir $(OBJ))
	@echo -e "\t[MK] $(dir $(OBJ))"

all: dirs build/$(PROJECT_NAME)

clear:
	rm -rf build
