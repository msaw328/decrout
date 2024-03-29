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

// parser - Parsing functionality for AST primitives

#ifndef _I_PARSER_PARSER_H_
#define _I_PARSER_PARSER_H_

#include <stdio.h>

#include "lexer/token_list.h"
#include "ast/ast.h"

// Processes the token list and generates AST
int parser_process_token_list(lexer_token_list_t* list, ast_global_scope_t* ast);

// Output from the parsing stage
void parser_write_output(FILE* outfile, ast_global_scope_t* ast);

#endif
