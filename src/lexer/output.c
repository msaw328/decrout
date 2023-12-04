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

// output - Printing output of the lexing stage

#include <stdio.h>

#include "token_list.h"

void lexer_write_output(FILE* outfile, lexer_token_list_t* list) {
    lexer_token_iterator_t iter = { 0 };
    lexer_token_list_into_iter(list, &iter);

    lexer_token_t* tk = NULL;
    while((tk = lexer_token_iter_next(&iter)) != NULL) {
        fprintf(outfile, "Token {\n\ttype - %u\n\tcontents - %s\n\tline - %zu\n\tchar - %zu\n}\n",
            tk->type, tk->contents != NULL ? tk->contents : "(empty)", tk->line_ref, tk->char_ref
        );
    }
}
