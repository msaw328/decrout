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

#ifndef _I_LEXER_LEXER_H_
#define _I_LEXER_LEXER_H_

#include <stdlib.h>
#include <stddef.h>

#include "token_list.h"

// Walks through a null-terminated source code string pointed to by
// src_str and returns a token list (structure pointed to by list is modified)
//
// Return value: 0 if ok, 1 if error
int lexer_process_source_code(char* src_str, lexer_token_list_t* list);

#endif
