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

// output - Printing output of the parsing stage

#include <stdio.h>

#include "utils/list.h"
#include "ast/ast.h"

void parser_write_output(FILE* outfile, ast_global_scope_t* ast) {
    fprintf(outfile, "Global {");

    for(size_t idx = 0; idx < UTILS_LIST_GENERIC_LENGTH(ast->decls); idx++) {
        ast_decl_t* decl = UTILS_LIST_GENERIC_GET(ast->decls, idx);

        char* type_str = decl->type != NULL ? type_to_string(decl->type) : "(to infer)";

        fprintf(outfile, "\n\tDeclaration {\n\t\tsymbol - %s\n\t\tis const - %d\n\t\ttype - %s\n\t\tline - %zu\n\t\tchar - %zu\n\t}",
            decl->symbol, decl->is_const, type_str, decl->line_ref, decl->char_ref
        );

        if(decl->type != NULL) free(type_str);
    };

    fprintf(outfile, "\n}\n");
}
