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

// parser - Parsing functionality for AST primitives

#include "parser.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lexer/token_list.h"
#include "lexer/token_types.h"
#include "types/types.h"
#include "ast/ast.h"

#include "parse_types.h"
#include <stdio.h>
// Assumes iterator points to the first token of a declaration
// Consumes tokens until declaration is fully described
// Returns NULL if error, or new declaration if ok
ast_decl_t* parser_parse_declaration(lexer_token_iterator_t* iter) {
    ast_decl_t* new_decl = malloc(sizeof(ast_decl_t));

    lexer_token_t* token = lexer_token_iter_next(iter);

    switch(token->type) {
        case TOKEN_CONST:
            new_decl->is_const = 1;
            break;

        case TOKEN_DECL:
            new_decl->is_const = 0;
            break;

        default: {
            fprintf(stderr, "[parser] Error in line %zu char %zu: Unexpected token at the beginning of a declaration, expected 'const' or 'decl'.\n", token->line_ref, token->char_ref);
            free(new_decl);
            return NULL;
        }
    }

    // Declarations takes its references from the const/decl keyword
    new_decl->line_ref = token->line_ref;
    new_decl->char_ref = token->char_ref;

    token = lexer_token_iter_next(iter);

    if(token == NULL) {
        fprintf(stderr, "[parser] Error in declaration in line %zu char %zu: Unexpected end of file.\n", new_decl->line_ref, new_decl->char_ref);
        free(new_decl);
        return NULL;
    }

    // We expect the identifier now
    if(token->type != TOKEN_IDENTIFIER || token->contents == NULL) {
        fprintf(stderr, "[parser] Error in declaration in line %zu char %zu: Expected identifier.\n", new_decl->line_ref, new_decl->char_ref);
        free(new_decl);
        return NULL;
    }

    // Copy the identifier to decl structure
    new_decl->symbol = malloc(strlen(token->contents) + 1);
    strcpy(new_decl->symbol, token->contents);

    token = lexer_token_iter_next(iter);

    if(token == NULL) {
        fprintf(stderr, "[parser] Error in declaration in line %zu char %zu: Unexpected end of file.\n", new_decl->line_ref, new_decl->char_ref);
        free(new_decl->symbol);
        free(new_decl);
        return NULL;
    }

    // We expect a separator, either : for type definition or = to infer type and provide expression
    if(token->type == TOKEN_COLON) {
        // Declaration contains type information

        if(!lexer_token_iter_isnt_empty(iter)) {
            fprintf(stderr, "[parser] Error in declaration in line %zu char %zu: Unexpected end of file.\n", new_decl->line_ref, new_decl->char_ref);
            free(new_decl->symbol);
            free(new_decl);
            return NULL;
        }

        // Parse the type and handle errors
        new_decl->type = parser_parse_type(iter);
        if(new_decl->type == NULL) {
            fprintf(stderr, "[parser] Error in declaration in line %zu char %zu: Cannot parse type.\n", new_decl->line_ref, new_decl->char_ref);
            free(new_decl->symbol);
            free(new_decl);
            return NULL;
        }
        token = lexer_token_iter_next(iter);
    } else {
        // Declaration is immediately followed by value, skipping
        // type info. Leave type to be inferred later.
        new_decl->type = NULL;
    }

    if(token == NULL) {
        fprintf(stderr, "[parser] Error in declaration in line %zu char %zu: Unexpected end of file.\n", new_decl->line_ref, new_decl->char_ref);
        type_destroy(new_decl->type);
        free(new_decl->symbol);
        free(new_decl);
        return NULL;
    }

    // If it is followed by semicolon it means were done with current decl, can return
    if(token->type == TOKEN_SEMICOLON) {
        return new_decl;
    }

    // If it is NOT followed by '=' its an error, you either end declaration or provide value
    if(token->type != TOKEN_EQUAL) {
        fprintf(stderr, "[parser] Error in declaration in line %zu char %zu: Expected value or end of declaration.\n", new_decl->line_ref, new_decl->char_ref);
        type_destroy(new_decl->type);
        free(new_decl->symbol);
        free(new_decl);
        return NULL;
    }

    // TODO: Parse body here
    // For now lets just consume all tokens until semicolon ends declaration
    do {
        token = lexer_token_iter_next(iter);
        if(token == NULL) {
            fprintf(stderr, "[parser] Error in declaration in line %zu char %zu: Unexpected end of file.\n", new_decl->line_ref, new_decl->char_ref);
            type_destroy(new_decl->type);
            free(new_decl->symbol);
            free(new_decl);
            return NULL;
        }
    } while(token->type != TOKEN_SEMICOLON);

    return new_decl;
}

// Processes the token list and generates AST
// TODO: Parse more than just declarations, add expressions etc
int parser_process_token_list(lexer_token_list_t* list, ast_global_scope_t* ast) {
    // Create an iterator over the list's contents.
    // The iterator is reference-only. The list cannot be destroyed before the end of iteration.
    lexer_token_iterator_t iter;
    lexer_token_list_into_iter(list, &iter);

    ast_decl_list_t* decls = ast->decls;
    while(lexer_token_iter_isnt_empty(&iter)) {
        ast_decl_t* new_decl = parser_parse_declaration(&iter);

        if(new_decl == NULL) {
            fprintf(stderr, "%s", "[parser] Error during parsing of global declarations.\n");
            return 1;
        }

        ast_decl_list_append(decls, new_decl);
    }

    ast->decls = decls;

    return 0;
}
