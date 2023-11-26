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

// parse_types - Parsing of type decalrations
// Assumes iterator points to the first token of a type
// Consumes tokens until type is fully described
// Returns NULL if error, or new declaration if ok

#include "parse_types.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lexer/token_list.h"
#include "lexer/token_types.h"
#include "types/types.h"
#include "types/type_list.h"
#include "ast/ast.h"

// Parse the [] portion of a routine type, starting from the first
// token after '['
type_info_list_t* parser_parse_routine_type_args(lexer_token_iterator_t* iter) {
    // We need to peek here, because its either ']' which ends args
    // Or it is important for the nested _parse_type call
    lexer_token_t* token = lexer_token_iter_peek(iter);

    if(token == NULL) {
        fprintf(stderr, "%s", "[parser] Error: Unexpected end of file.\n");
        return NULL;
    }

    // empty argument list is still valid, cause NULL means error
    type_info_list_t* args = type_info_list_make();
    // Check if arg list is not followed by ']' immediately, dont allocate a list of args in such case
    if(token->type != TOKEN_END_SQUARE) {

        while(1) {
            type_info_t* arg_type = parser_parse_type(iter);

            if(arg_type == NULL) {
                fprintf(stderr, "[parser] Error in line %zu char %zu: Unable to parse type.\n", token->line_ref, token->char_ref);
                type_info_list_destroy(args);
                return NULL;
            }

            type_info_list_append(args, arg_type);

            token = lexer_token_iter_next(iter);

            if(token == NULL) {
                fprintf(stderr, "%s", "[parser] Error: Unexpected end of file.\n");
                type_info_list_destroy(args);
                return NULL;
            }

            // If it's ']' we leave the arg list
            if(token->type == TOKEN_END_SQUARE) {
                break;
            }

            // Each arg except the last must be followed by ','
            if(token->type != TOKEN_COMMA) {
                fprintf(stderr, "[parser] Error in line %zu char %zu: Unexpected token, expected comma\n", token->line_ref, token->char_ref);
                type_info_list_destroy(args);
                return NULL;
            }
        }
    } else {
        lexer_token_iter_next(iter);
    }

    return args;
}

// Parse the routine type, starting from first toke nafter 'rt'
type_info_t* parser_parse_routine_type(lexer_token_iterator_t* iter) {
    type_info_list_t* args = NULL;
    type_info_t* ret = NULL;

    // Next token is either '[' if there are args, or ':' if no args, just return type
    lexer_token_t* token = lexer_token_iter_next(iter);

    if(token == NULL) {
        fprintf(stderr, "%s", "[parser] Error: Unexpected end of file.\n");
        return NULL;
    }

    // If it's '[' means routine accepts arguments, if its not it HAS to be ':' for return type
    if(token->type == TOKEN_SQUARE) {
        args = parser_parse_routine_type_args(iter);

        if(args == NULL) {
            fprintf(stderr, "[parser] Error in line %zu char %zu: Unable to parse argument list.\n", token->line_ref, token->char_ref);
            return NULL;
        }

        // Put the next token into the variable (should be ':')
        token = lexer_token_iter_next(iter);
    };

    if(token == NULL) {
        fprintf(stderr, "%s", "[parser] Error: Unexpected end of file.\n");
        type_info_list_destroy(args);
        return NULL;
    }

    // Has to be ':'
    if(token->type != TOKEN_COLON) {
        fprintf(stderr, "[parser] Error in line %zu char %zu: Expected ':'.\n", token->line_ref, token->char_ref);
        type_info_list_destroy(args);
        return NULL;
    }

    ret = parser_parse_type(iter);

    if(ret == NULL) {
        fprintf(stderr, "%s", "[parser] Error while parsing routine type: Unable to parse type.\n");
        type_info_list_destroy(args);
        return NULL;
    }

    return type_make_routine(args, ret);
}

type_info_t* parser_parse_type(lexer_token_iterator_t* iter) {
    type_info_t* parsed_type = NULL;

    lexer_token_t* token = lexer_token_iter_next(iter);

    switch(token->type) {
        // 'rt' keyword means routine
        case TOKEN_RT: {
            parsed_type = parser_parse_routine_type(iter);
            if(parsed_type == NULL) {
                fprintf(stderr, "[parser] Error in line %zu char %zu: Unable to parse routine type.\n", token->line_ref, token->char_ref);
                return NULL;
            }
            break;
        }

        // '>' means a pointer
        case TOKEN_TRIANGLE_RIGHT: {
            type_info_t* type_pointed_to = parser_parse_type(iter);
            if(type_pointed_to == NULL) {
                fprintf(stderr, "[parser] Error in line %zu char %zu: Unable to parse pointer type.\n", token->line_ref, token->char_ref);
                return NULL;
            }
            parsed_type = type_make_pointer_to(type_pointed_to);
            break;
        }

        // identifier means a builtin type (or structural once those are implemented)
        case TOKEN_IDENTIFIER: {
            // TODO: Handle structural types here once they are implemented
            parsed_type = type_get_builtin_by_name(token->contents);
            if(parsed_type == NULL) {
                fprintf(stderr, "[parser] Error in line %zu char %zu: Unable to parse type '%s'.\n", token->line_ref, token->char_ref, token->contents);
                return NULL;
            }
            break;
        }

        // Unknown token
        default: {
            fprintf(stderr, "[parser] Error in line %zu char %zu: Unexpected token, expected type.\n", token->line_ref, token->char_ref);
            return NULL;
        }
    }

    return parsed_type;
}
