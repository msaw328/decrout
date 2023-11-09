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

// token_list - a list of tokens that can be iterated over, created from a memory buffer of source code

#ifndef _I_LEXER_TOKEN_LIST_H_
#define _I_LEXER_TOKEN_LIST_H_

#include <stddef.h>

#include "token_types.h"

// Token has a type. Some types might have varying contents (stinrg literals, numeric literals).
// If that is the case, the contents are saved in a malloc'ed buffer in token_contents.
struct lexer_token_t {
    lexer_token_type_t type;    // Type of a token (token_tyes.h)
    char* contents;             // If a token has dynamic contents, malloc'ed buffer
    size_t line_ref;            // Line number in file
    size_t char_ref;            // Character number in line (in file)
};
typedef struct lexer_token_t lexer_token_t;

// A list of tokens, it is created from a memory buffer containing source code
struct lexer_token_list_t {
    size_t num_tokens;          // Number of tokens in the list
    size_t alloc_tokens;        // Number of allocated tokens in array
    lexer_token_t* arr;         // Malloc'ed array of tokens, of size alloc_tokens
};
typedef struct lexer_token_list_t lexer_token_list_t;

// An iterator over a list of tokens
struct lexer_token_iterator_t {
    lexer_token_list_t* list;   // Keeps reference to the list
    size_t next_index;          // Keeps track of the next index to retrieve
};
typedef struct lexer_token_iterator_t lexer_token_iterator_t;

// Initialize the list to sensible defaults
void lexer_token_list_init(lexer_token_list_t* l);

// Append a new token to the list (data is copied from the structure)
void lexer_token_list_append(lexer_token_list_t* l, lexer_token_t* tk);

// Destroy dynamic memory associated with the list
void lexer_token_list_destroy(lexer_token_list_t* l);

// Creates a reference-only iterator over the list
// WARNING the created iterator only references the list, do not destroy it before finishing iteration
void lexer_token_list_into_iter(lexer_token_list_t* l, lexer_token_iterator_t* iter);

// Returns pointer to the next token structure, or NULL when out of tokens
lexer_token_t* lexer_token_iterator_next(lexer_token_iterator_t* iter);

#endif
