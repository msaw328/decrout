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

#include "token_list.h"

#include <stdlib.h>
#include <string.h>

#define DEFAULT_LIST_SIZE 32

void __list_grow(lexer_token_list_t* l) {
    l->alloc_tokens = 2 * l->alloc_tokens;
    lexer_token_t* new_arr = malloc(l->alloc_tokens * sizeof(lexer_token_t));
    memcpy(new_arr, l->arr, l->num_tokens * sizeof(lexer_token_t));
    free(l->arr);
    l->arr = new_arr;
}

// Initialize the list to sensible defaults
void lexer_token_list_init(lexer_token_list_t* l) {
    l->num_tokens = 0;
    l->alloc_tokens = DEFAULT_LIST_SIZE;
    l->arr = malloc(DEFAULT_LIST_SIZE * sizeof(lexer_token_t));
}

// Append a new token to the list (token structure is copied to the array)
void lexer_token_list_append(lexer_token_list_t* l, lexer_token_t* token) {
    if(l->num_tokens >= l->alloc_tokens) {
        __list_grow(l);
    }

    memcpy(&(l->arr[l->num_tokens]), token, sizeof(lexer_token_t));

    l->num_tokens += 1;
}

// Destroy dynamic memory associated with the list
void lexer_token_list_destroy(lexer_token_list_t* l) {
    for(size_t i = 0; i < l->alloc_tokens; i++) {
        lexer_token_t* tk = &(l->arr[i]);
        if(TOKEN_TYPE_IS_DYNAMIC(tk->type)) {
            free(tk->contents);
        }
    };

    free(l->arr);
}

// Creates a reference-only iterator over the list
// WARNING the created iterator only references the list, do not destroy it before finishing iteration
void lexer_token_list_into_iter(lexer_token_list_t* l, lexer_token_iterator_t* iter) {
    iter->list = l;
    iter->next_index = 0;
}

// Returns pointer to the next token structure, or NULL when out of tokens
lexer_token_t* lexer_token_iterator_next(lexer_token_iterator_t* iter) {
    if(iter->next_index < iter->list->num_tokens) {
        lexer_token_t* tk = &(iter->list->arr[iter->next_index]);
        iter->next_index += 1;
        return tk;
    } else {
        return NULL;
    }
}
