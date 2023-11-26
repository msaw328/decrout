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

// ast - AST structures, used during parsing

// All of the structures here own all their data, unless otherwise noted (for example - for static builtin type infos)
// this means that destruction should free all owned resources
// The topmost structure should be the ast_global_scope_t

#ifndef _I_AST_AST_H_
#define _I_AST_AST_H_

#include <stddef.h>

#include "types/types.h"
#include "decl_list.h"

// Topmost structure of the AST, containing the global scope
// The global scope may contain only declarations!
struct ast_global_scope_t {
    struct ast_decl_list_t* decls;
};
typedef struct ast_global_scope_t ast_global_scope_t;

// Structure which describes a declaration
struct ast_decl_t {
    size_t line_ref; // line pos and char pos point to the const/decl keyword position in the file for further error reporting
    size_t char_ref;
    int is_const; // 1 - Const or 0 - non-const
    type_info_t* type; // Declaration has a type, or if the type is meant to be inferred this could perhaps be NULL
    char* symbol; // Symbol name string
    // TODO: Here should be a field of type ast_expr_t* etc etc
};
typedef struct ast_decl_t ast_decl_t;

// TODO: Model expressions, statements, scopes and all that here

ast_global_scope_t* ast_global_scope_make();
void ast_global_scope_destroy(ast_global_scope_t*);

#endif
