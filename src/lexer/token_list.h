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

#include "token_types.h"

#include "utils/list.h"

#include <stddef.h>

// Token has a type. Some types might have varying contents (stinrg literals, numeric literals).
// If that is the case, the contents are saved in a malloc'ed buffer in token_contents.
// Other times, for a static token, the contents buffer is not used.
// During cleanup of the token structure, the TOKEN_TYPE_IS_DYNAMIC macro from token_types.h should be used.
struct lexer_token_t {
    lexer_token_type_t type;    // Type of a token (token_tyes.h)
    char* contents;             // If a token has dynamic contents, malloc'ed buffer
    size_t line_ref;            // Line number in file
    size_t char_ref;            // Character number in line (in file)
};
typedef struct lexer_token_t lexer_token_t;

UTILS_LIST_MAKE_DECLARATION(lexer_token, struct lexer_token_t)

// An iterator over a list of tokens
struct lexer_token_iterator_t {
    lexer_token_list_t* list;   // Keeps reference to the list
    size_t next_index;          // Keeps track of the next index to retrieve
};
typedef struct lexer_token_iterator_t lexer_token_iterator_t;

// Creates a reference-only iterator over the list
// WARNING the created iterator only references the list, do not destroy it before finishing iteration
void lexer_token_list_into_iter(lexer_token_list_t* l, lexer_token_iterator_t* iter);

// Returns pointer to the next token structure, or NULL when out of tokens
lexer_token_t* lexer_token_iter_next(lexer_token_iterator_t* iter);

// Returns 1 if theres a next element or 0 if iter empty
int lexer_token_iter_isnt_empty(lexer_token_iterator_t* iter);

// Return next token without removing it from the iterator (or NULL)
lexer_token_t* lexer_token_iter_peek(lexer_token_iterator_t* iter);

#endif
