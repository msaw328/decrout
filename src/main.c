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

#include "lexer/lexer.h"
#include "io/fileread.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
#if 1
    if(argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 0;
    }

    // Retrieve source code from somewhere (in this case, a file)
    // Store it as a null-terminated string
    char* filecontents = io_read_source_file(argv[1]);
    if(filecontents == NULL) {
        return 0;
    }

    // Initialize a list of tokens
    lexer_token_list_t list = { 0 };
    lexer_token_list_init(&list);

    // Process the source code, filling the list of tokens
    // Token data is copied, where necessary so one may immediately free source code buffer
    int result = lexer_process_source_code(filecontents, &list);
    free(filecontents);

    // Error checking
    if(result != 0) {
        return 0;
    }

    // Create an iterator over the list's contents.
    // The iterator is reference-only. The list cannot be destroyed before the end of iteration.
    lexer_token_iterator_t iter;
    lexer_token_list_into_iter(&list, &iter);

    // Iterate over the stream of tokens. End of the iterator/list is signaled by returning NULL.
    lexer_token_t* tk = NULL;
    while((tk = lexer_token_iterator_next(&iter)) != NULL) {
        printf("type: %d\ncontents: %s\nline: %zu\nchar: %zu\n\n", tk->type, tk->contents, tk->line_ref, tk->char_ref);
    }

    // After all tokens have been consumed, destroy the list.
    lexer_token_list_destroy(&list);
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
