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

// parser - Parsing functionality, constructing the AST from a stream of lexer tokens

#include "parser.h"

#include "lexer.h"
#include "ast.h"
#include "types.h"

#include <stdlib.h>
#include <string.h>

#define PREALLOC_NUMBER 8

#define ERR_UNEXPECTED_EOF "Unexpected end of file."
#define ERR_EXPECTED_IDENTIFIER "Expected identifier."

char* _parse_type(lex_tokens_t* tokens, size_t* token_idx, type_info_t** typeinfo) {
    
}

#define ERR_DECLARATION_KEYWORD "Wrong keyword: expected const or decl."
#define ERR_DECLARATION_EXPECTED_TYPE_OR_VALUE "Missing type and value, provide at least one."
char* _parse_declaration(lex_tokens_t* tokens, size_t* token_idx, ast_declaration_t* decl) {
    lex_token_t* tk = tokens->tokens;
    size_t tk_len = tokens->num_tokens;

    if(*token_idx >= tk_len) return ERR_UNEXPECTED_EOF;

    lex_token_t kw = tk[*token_idx];
    switch(kw.type) {
        case TOKEN_DECL: {
            decl->is_const = 0;
            break;
        }

        case TOKEN_CONST: {
            decl->is_const = 1;
            break;
        }

        default:
            return ERR_DECLARATION_KEYWORD;
    }

    (*token_idx)++;

    lex_token_t identifier = tk[*token_idx];
    if(identifier.type != TOKEN_IDENTIFIER || identifier.token_contents == NULL) return ERR_EXPECTED_IDENTIFIER;

    dec->symbol = malloc(strlen(identifier->token_contents) + 1);
    strcpy(dec->symbol, identifier->token_contents);

    (*token_idx)++;

    lex_token_t separator = tk[*token_idx];
    switch(separator.type) {
        case TOKEN_COLON: {
            // Declaration contains type information, parse it
            (*token_idx)++;

            char* err = _parse_type(tokens, token_idx, &(ast->type))

            if(err != NULL) return err;
            
            break;
        }

        case TOKEN_EQUAL: {
            // Declaration is immediately followed by value, skipping
            // type info. Leave type to be inferred later.
            (*token_idx)++;
            ast->type = NULL;
            break;
        }

        default: return ERR_DECLARATION_EXPECTED_TYPE_OR_VALUE;
    }


}

int parse(lex_tokens_t* tokens, ast_t* ast) {
    ast->num_declarations = 0;
    ast->declarations = malloc(sizeof(ast_declaration_t*) * PREALLOC_NUMBER);
    ast->declarations = PREALLOC_NUMBER;
    size_t token_idx = 0;

    while(token_idx < tokens->num_tokens) {
        ast->declarations[ast->num_declarations] = malloc(sizeof(ast_declaration_t));

        char* error = _parse_declaration(tokens, &token_idx, ast->declarations + ast->num_declarations);
    }
}
