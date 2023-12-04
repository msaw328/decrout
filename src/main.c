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
#include "context/args.h"

#include <stdio.h>
#include <string.h>



int main(int argc, char** argv) {
#if 1
    context_args_t* args = context_args_parse(argc, argv);
    if(args == NULL) {
        return 0;
    }

    // Retrieve source code from somewhere (in this case, a file)
    // Store it as a null-terminated string
    char* filecontents = io_read_source_file(args->input_file);
    if(filecontents == NULL) {
        free(args);
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
        free(args);
        return 0;
    }

    if(args->output_stage == STAGE_LEXER) {
        lexer_write_output(args->output_file, list);
        free(args);
        return 0;
    }

    ast_global_scope_t* ast = ast_global_scope_make();
    result = parser_process_token_list(list, ast);

    // After all tokens have been consumed, destroy the list.
    lexer_token_list_destroy(list);

    // Error checking
    if(result != 0) {
        free(args);
        ast_global_scope_destroy(ast);
        return 0;
    }

    if(args->output_stage == STAGE_PARSER) {
        parser_write_output(args->output_file, ast);
        ast_global_scope_destroy(ast);
        free(args);
        return 0;
    }

    // TODO: After parsing is finished move to next stage (probably validity checks?)

    free(args);
    ast_global_scope_destroy(ast);
#endif
}
