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

// main - Entry point of the program

#include "lexer.h"
#include "fileread.h"
#include "parser.h"

#include "ast.h"
#include "types.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 0;
    }

#if 1
    char* filecontents = read_source_file(argv[1]);

    lex_tokens_t tk = { 0 };
    tokenize_and_lex(filecontents, &tk);

    ast_t ast = { 0 };
    parse(&tk, &ast);

    for(size_t idx = 0; idx < ast.num_declarations; idx++) {
        ast_declaration_t* decl = ast.declarations[idx];
        char* type_str = type_to_string(decl->type);
        printf("Decl %zu ::: Const? %d Sym: \"%s\" Type: |%s| Value: %p\n", idx, decl->is_const, decl->symbol, type_str, (void*) decl->value);
        free(type_str);
    }

    // TODO: Create cleanup functions instead of these
    free(tk.tokens);
    free(filecontents);
#endif

#if 0
    size_t arg_count = 3;

    type_info_t** arg_types = malloc(sizeof(type_info_t*) * arg_count);
    arg_types[0] = type_get_builtin_by_name("u32");
    arg_types[1] = type_make_pointer_to(type_get_builtin_by_name("u32"));
    arg_types[2] = type_make_pointer_to(type_make_pointer_to(type_get_builtin_by_name("u64")));

    type_info_t* return_type = type_make_pointer_to(type_get_builtin_by_name("u32"));

    type_info_t* routine_type = type_make_routine(arg_count, arg_types, return_type);
    type_info_t* total_type = type_make_pointer_to(routine_type);
    char* type_name = type_to_string(total_type);
    printf("Type name: %s\n", type_name);

    type_destroy(total_type);
    free(type_name);
#endif
}
