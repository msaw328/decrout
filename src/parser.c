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
#include <stdio.h>

#define PREALLOC_NUMBER 4

#define ERR_REALLOC "Error during realloc."
#define ERR_UNEXPECTED_EOF "Unexpected end of file."
#define ERR_EXPECTED_IDENTIFIER "Expected identifier."

#define ERR_UNEXPECTED_TOKEN_IN_TYPE "Unexpected token in type declaration."
char* _parse_type(lex_token_t* tk, size_t tk_len, size_t* token_idx, type_info_t** typeinfo) {
    lex_token_t* first_type_token = &(tk[*token_idx]);

    type_info_t* parsed_type = NULL;

    switch(first_type_token->type) {
        // 'rt' keyword, means a routine definition
        case TOKEN_RT: {
            if(*token_idx >= tk_len) return ERR_UNEXPECTED_EOF;
            (*token_idx)++;

            // Opening arg list
            if(tk[*token_idx].type != TOKEN_SQUARE) return ERR_UNEXPECTED_TOKEN_IN_TYPE;

            if(*token_idx >= tk_len) return ERR_UNEXPECTED_EOF;
            (*token_idx)++;

            type_info_t** args = NULL;
            size_t arg_c = 0;

            // Check if there are any args at all
            if(tk[*token_idx].type != TOKEN_END_SQUARE) {
                size_t alloc_args = PREALLOC_NUMBER;
                args = malloc(sizeof(type_info_t*) * alloc_args);

                // Parse arguments
                while(1) {
                    // If we do not have enough space for the next argument, reallocate arg list
                    if(arg_c + 1 > alloc_args) {
                        alloc_args = 2 * alloc_args;
                        if(NULL == realloc(args, sizeof(type_info_t*) * alloc_args)) return ERR_REALLOC;
                    }

                    // parse the next arg type
                    char* err = _parse_type(tk, tk_len, token_idx, &(args[arg_c]));

                    if(err != NULL) return err;
                    arg_c++;

                    // End arg list on ']', continue on ',', return error on anything else
                    if(tk[*token_idx].type == TOKEN_END_SQUARE) break;
                    if(tk[*token_idx].type == TOKEN_COMMA) {
                        if(*token_idx >= tk_len) return ERR_UNEXPECTED_EOF;
                        (*token_idx)++;
                        continue;
                    };

                    return ERR_UNEXPECTED_TOKEN_IN_TYPE;
                }
            }

            if(*token_idx >= tk_len) return ERR_UNEXPECTED_EOF;
            (*token_idx)++;

            // Now is the return type, starting with a colon
            if(tk[*token_idx].type != TOKEN_COLON) return ERR_UNEXPECTED_TOKEN_IN_TYPE;

            if(*token_idx >= tk_len) return ERR_UNEXPECTED_EOF;
            (*token_idx)++;

            type_info_t* return_type;
            char* err = _parse_type(tk, tk_len, token_idx, &return_type);

            if(err != NULL) return err;

            parsed_type = type_make_routine(arg_c, args, return_type);

            break;
        }

        // '>', means a pointer
        case TOKEN_TRIANGLE_RIGHT: {
            if(*token_idx >= tk_len) return ERR_UNEXPECTED_EOF;
            (*token_idx)++;
            type_info_t* inner_type = NULL;
            char* err = _parse_type(tk, tk_len, token_idx, &inner_type);

            if(err != NULL) return err;

            parsed_type = type_make_pointer_to(inner_type);

            break;
        }

        // identifier, most likely means a builtin type (or structura type after those are implemented)
        case TOKEN_IDENTIFIER: {
            // TODO: differentiate between builtin types and structures once those are implemented
            // For now only support builtin types
            parsed_type = type_get_builtin_by_name(first_type_token->token_contents);
            if(*token_idx >= tk_len) return ERR_UNEXPECTED_EOF;
            (*token_idx)++;
            break;
        }

        default: return ERR_UNEXPECTED_TOKEN_IN_TYPE;
    }

    *typeinfo = parsed_type;

    return NULL;
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

    if(*token_idx >= tk_len) return ERR_UNEXPECTED_EOF;
    (*token_idx)++;

    lex_token_t* identifier = &(tk[*token_idx]);
    if(identifier->type != TOKEN_IDENTIFIER || identifier->token_contents == NULL) return ERR_EXPECTED_IDENTIFIER;

    decl->symbol = malloc(strlen(identifier->token_contents) + 1);
    strcpy(decl->symbol, identifier->token_contents);


    if(*token_idx >= tk_len) return ERR_UNEXPECTED_EOF;
    (*token_idx)++;

    lex_token_t* separator = &(tk[*token_idx]);
    if(separator->type == TOKEN_COLON) {
        // Declaration contains type information, parse it
        if(*token_idx >= tk_len) return ERR_UNEXPECTED_EOF;
        (*token_idx)++;

        char* err = _parse_type(tk, tk_len, token_idx, &(decl->type));
        if(err != NULL) return err;
    } else {
        // Declaration is immediately followed by value, skipping
        // type info. Leave type to be inferred later.
        decl->type = NULL;
    }
    
    // If we end up on a semicolon here it means there is no body for the declaration
    // TODO: Not sure if check for a body in a const declaration should happen here
    if(tk[*token_idx].type == TOKEN_SEMICOLON) {
        decl->value = NULL;
        if(*token_idx >= tk_len) return ERR_UNEXPECTED_EOF;
        (*token_idx)++;
        return NULL;
    }

    if(tk[*token_idx].type == TOKEN_EQUAL) return ERR_DECLARATION_EXPECTED_TYPE_OR_VALUE;

    // TODO: Parse body here
    do {
        if(*token_idx >= tk_len) return ERR_UNEXPECTED_EOF;
        (*token_idx)++;
    } while(tk[*token_idx].type != TOKEN_SEMICOLON);

    return NULL;
}

void parse(lex_tokens_t* tokens, ast_t* ast) {
    ast->num_declarations = 0;
    ast->declarations = malloc(sizeof(ast_declaration_t*) * PREALLOC_NUMBER);
    ast->allocated_declarations = PREALLOC_NUMBER;
    size_t token_idx = 0;

    while(token_idx < tokens->num_tokens) {
        // Skip empty statements
        if(tokens->tokens[token_idx].type == TOKEN_SEMICOLON) {
            token_idx++;
            continue;
        }

        if(ast->num_declarations >= ast->allocated_declarations) {
            ast->allocated_declarations = 2 * ast->allocated_declarations;
            ast->declarations = realloc(ast->declarations, sizeof(ast_declaration_t*) * ast->allocated_declarations);
        }

        ast->declarations[ast->num_declarations] = malloc(sizeof(ast_declaration_t));

        char* error = _parse_declaration(tokens, &token_idx, ast->declarations[ast->num_declarations]);
        if(error != NULL) {
            printf("Error parsing at line %zu: %s\n", tokens->tokens[token_idx].line_ref, error);
            exit(0);
        };

        ast->num_declarations++;
    }
}
