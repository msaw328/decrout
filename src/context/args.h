// decrout - declare routine - language one level above assembly
// Copyright (C) 2023  Maciej Sawka <maciejsawka@gmail.com> <msaw328@kretes.xyz>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// args - General cli argument handling

#ifndef _I_CONTEXT_ARGS_H_
#define _I_CONTEXT_ARGS_H_

#include <stdio.h>

// Enumeration for constants defining compiler passes
enum context_stage_t {
#define STAGE_FIRST STAGE_LEXER
    STAGE_LEXER = 0,
    STAGE_PARSER,
#define STAGE_LAST STAGE_PARSER
};
typedef enum context_stage_t context_stage_t;

// Structure which contains CLI flags which modify compiler behavior
struct context_args_t {
    context_stage_t output_stage;   // After which stage should compiler output
    FILE* output_file;              // FILE* to write output to
    FILE* input_file;               // FILE* to read input from
};
typedef struct context_args_t context_args_t;

// The args structure is malloc'ed - requires freeing
context_args_t* context_args_parse(int argc, char** argv);

#endif
