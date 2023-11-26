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

#include "ast.h"

#include <stdlib.h>

#include "decl_list.h"

ast_global_scope_t* ast_global_scope_make() {
    ast_global_scope_t* ast = malloc(sizeof(ast_global_scope_t));

    ast->decls = ast_decl_list_make();

    return ast;
}

void ast_global_scope_destroy(ast_global_scope_t* ast) {
    ast_decl_list_destroy(ast->decls);
    free(ast);
}
