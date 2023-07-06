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

#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 0;
    }

    char* filecontents = read_source_file(argv[1]);

    lex_tokens_t tk = { 0 };
    tokenize_and_lex(filecontents, &tk);

    // TODO: Add parsing
    // Replace below testing code for tokens with proper parsing

    printf("Total tokens: %zu\n", tk.num_tokens);
    for(size_t i = 0; i < tk.num_tokens; i++) {
        printf("Token type: %02X Token contents: %s\n", tk.tokens[i].type, (tk.tokens[i].token_contents != NULL) ? tk.tokens[i].token_contents : "(null)");
        if(tk.tokens[i].token_contents != NULL) {
            free(tk.tokens[i].token_contents);
        }
    }

    free(tk.tokens);
    free(filecontents);
}
