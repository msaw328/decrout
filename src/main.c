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

#include "io/fileread.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "types/types.h"
#include "utils/list.h"

#include "version.h"

#include <stdio.h>
#include <string.h>

void print_usage_and_exit(char** argv) {
    puts("dcrtc - Decrout compiler");
    printf("Usage: %s <filename>\n", argv[0]);
    printf("%s -h for help, %s -v for version information\n", argv[0], argv[0]);
    exit(0);
}

void print_version_and_exit() {
    puts("dcrtc - Decrout compiler");
    puts("Copyright (C) 2023  Maciej Sawka <maciejsawka@gmail.com> <msaw328@kretes.xyz>");
    printf("Commit id: %s\nBuild date: %s\n\n", DCRTC_COMMIT_ID, DCRTC_BUILD_DATE);
}

int main(int argc, char** argv) {
#if 1
    if(argc < 2 || strcmp(argv[1], "-h") == 0) {
        print_usage_and_exit(argv);
        return 0;
    }

    if(strcmp(argv[1], "-v") == 0) {
        print_version_and_exit();
        return 0;
    }

    // Retrieve source code from somewhere (in this case, a file)
    // Store it as a null-terminated string
    char* filecontents = io_read_source_file(argv[1]);
    if(filecontents == NULL) {
        return 0;
    }

    // Initialize a list of tokens
    lexer_token_list_t* list = lexer_token_list_make();

    // Process the source code, filling the list of tokens
    // Token data is copied, where necessary so one may immediately free source code buffer
    int result = lexer_process_source_code(filecontents, list);
    free(filecontents);

    // Error checking
    if(result != 0) {
        return 0;
    }

    ast_global_scope_t* ast = ast_global_scope_make();
    result = parser_process_token_list(list, ast);

#if 0
    for(size_t i = 0; i < UTILS_LIST_GENERIC_LENGTH(list); i++) {
        lexer_token_t* tk = UTILS_LIST_GENERIC_GET(list, i);
        printf("token %zu: type = %d, contents = %p\n", i, tk->type, (void*) tk->contents);
    }
#endif

    // After all tokens have been consumed, destroy the list.
    lexer_token_list_destroy(list);

    // Error checking
    if(result != 0) {
        ast_global_scope_destroy(ast);
        return 0;
    }

    ast_decl_list_t* decls = ast->decls;
    for(size_t i = 0; i < UTILS_LIST_GENERIC_LENGTH(decls); i++) {
        ast_decl_t* decl = UTILS_LIST_GENERIC_GET(decls, i);
        char* type_str = type_to_string(decl->type);
        printf("Decl %zu (ref %zu:%zu) ::: | Symbol: %s | Type: %s | Is const: %d |\n", 
            i,
            decl->line_ref,
            decl->char_ref,
            decl->symbol,
            type_str,
            decl->is_const
        );
        free(type_str);
    }

    ast_global_scope_destroy(ast);
#endif
}
