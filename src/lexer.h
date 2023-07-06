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

// lexer - Lexing functionality, recognition of tokens

#ifndef _I_LEXER_H_
#define _I_LEXER_H_

#include <stdlib.h>
#include <stddef.h>

#include "lex_token_types.h"

// Token has a type, and if the type might have varying contents,
// the contents are saved in a malloc'ed buffer in token_contents
struct lex_token_t {
    lex_token_type_t type;
    char* token_contents;
};
typedef struct lex_token_t lex_token_t;

struct lex_tokens_t {
    size_t num_tokens;
    lex_token_t* tokens;
};
typedef struct lex_tokens_t lex_tokens_t;

int tokenize_and_lex(char* src_str, lex_tokens_t* list);

#endif
