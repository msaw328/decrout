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

// lexer - Lexing functionality, recognition of tokens

#include "lexer.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static char* _predefined_tokens[] = {
    // First 3 are keywords, which require spaces around them
    // so they should be treated separately during tokenization,
    // but not during lexing
    [TOKEN_RT] = "rt",
    [TOKEN_DECL] = "decl",
    [TOKEN_RETURN] = "return",
    [TOKEN_CONST] = "const",
    [TOKEN_AT] = "@",
    [TOKEN_DOLLAR] = "$",
    [TOKEN_ASTERISK] = "*",
    [TOKEN_SLASH] = "/",
    [TOKEN_PLUS] = "+",
    [TOKEN_MINUS] = "-",
    [TOKEN_EQUAL] = "=",
    [TOKEN_TILDE] = "~",
    [TOKEN_AMPERSAND] = "&",
    [TOKEN_PIPE] = "|",
    [TOKEN_UP_ARROW] = "^",
    [TOKEN_EXCLAMATION] = "!",
    [TOKEN_DOUBLE_AMPERSAND] = "&&",
    [TOKEN_DOUBLE_PIPE] = "||",
    [TOKEN_DOUBLE_EQUAL] = "==",
    [TOKEN_TWO_TRIANGLES] = "<>",
    [TOKEN_TRIANGLE_LEFT] = "<",
    [TOKEN_TRIANGLE_RIGHT] = ">",
    [TOKEN_TRIANGLE_EQUAL_LEFT] = "<=",
    [TOKEN_TRIANGLE_EQUAL_RIGHT] = ">=",
    [TOKEN_DOT] = ".",
    [TOKEN_BRACKET] = "{",
    [TOKEN_END_BRACKET] = "}",
    [TOKEN_SQUARE] = "[",
    [TOKEN_END_SQUARE] = "]",
    [TOKEN_PAREN] = "(",
    [TOKEN_END_PAREN] = ")",
    [TOKEN_COLON] = ":",
    [TOKEN_SEMICOLON] = ";",
    [TOKEN_COMMA] = ","
};
#define PREDEFINED_TOKENS_NUM (sizeof(_predefined_tokens)/sizeof(_predefined_tokens[0]))

#define STARTING_TOKEN_LIST_LENGTH 32
#define STARTING_TOKEN_ALLOC_LENGTH 8
#define WHITESPACE_CHARS "\t\r\n "
#define DIGITS "0123456789"
// This function is only called after all the whitespace
// has been skipped, all comments omitted etc. so it is
// guaranteed that current char is a start of a valid token
//
// Its job is to read the next token and return it in a malloc'ed buffer
char* _read_token(char** src_str) {
    size_t allocated_token_size = STARTING_TOKEN_ALLOC_LENGTH;
    char* current_token = malloc(sizeof(char) * allocated_token_size);
    size_t current_token_length = 0;

    // This tracks whether we're in a char/string literal
    int is_in_string_or_char_literal = 0;
    char literal_type = ' '; // This tracks the type of literal (char or string)

    while(1) {
        // If we're in a string or a char literal we just eat up all the characters
        // as they go until a matching end for the literal is found
        // TODO: Recognize an escaped " or ' which should not end the literal
        if(is_in_string_or_char_literal) {
            current_token[current_token_length] = **src_str;
            current_token_length++;

            if(**src_str != literal_type) {
                (*src_str)++;
                continue;
            }

            current_token[current_token_length] = '\0';
            (*src_str)++;
            is_in_string_or_char_literal = 0;

            return current_token;
        }

        // If we're not in a string/char literal but we find a " or ' then we see
        // a beginning of a literal. If we have anything leftover then we should return it,
        // and leave the literal for the next iteration
        if(!is_in_string_or_char_literal && (**src_str == '\"' || **src_str == '\'')) {
            if(current_token_length != 0) {
                current_token[current_token_length] = '\0';
                return current_token;
            }

            is_in_string_or_char_literal = 1;
            literal_type = **src_str;
            current_token[current_token_length] = **src_str;
            current_token_length++;
            (*src_str)++;
            continue;
        }

        // If we find eof or a comment or whitespace we should finish current token
        // and let the parent function skip the comment for us or finish the file
        if(**src_str == '\0' || **src_str == '#' || strchr(WHITESPACE_CHARS, **src_str) != NULL) {
            current_token[current_token_length] = '\0';
            return current_token;
        }

        // Check if we found one of the predefined tokens that immediately finish the current token
        // Those are all predefined tokens except keywords in the beginning, which require space around them
        for(size_t i = TOKEN_AT; i < PREDEFINED_TOKENS_NUM; i++) {
            size_t len = strlen(_predefined_tokens[i]);

            if(memcmp(*src_str, _predefined_tokens[i], len) == 0) {
                if(current_token_length != 0) {
                    current_token[current_token_length] = '\0';
                } else {
                    memcpy(current_token, _predefined_tokens[i], len + 1);
                    (*src_str) += len;
                }

                return current_token;
            }

        }

        // If all of the above fails, it means its just a regular identifier
        // Check if we have enough for another character + a null byte, if not
        // reallocate bigger buffer
        if(current_token_length + 2 > allocated_token_size) {
            char* new_token_buff = malloc(sizeof(char) * 2 * allocated_token_size);
            memcpy(new_token_buff, current_token, current_token_length);
            free(current_token);
            current_token = new_token_buff;
            allocated_token_size = 2 * allocated_token_size;
        }

        // Finally, reach the character and try next one
        current_token[current_token_length] = **src_str;
        current_token_length++;

        (*src_str)++;
    }
}

void _determine_token_type(char* new_token, lex_token_t* tk) {
    for(size_t i = 0; i < PREDEFINED_TOKENS_NUM; i++) {
        if(strcmp(new_token, _predefined_tokens[i]) == 0) {
            tk->type = i;
            tk->token_contents = NULL;

            free(new_token);

            return;
        }
    }

    // if it is not predefined, determine literal/identifier type
    if(new_token[0] == '"') {
        tk->type = TOKEN_LITERAL_STRING;
        tk->token_contents = new_token;
        return;
    }

    if(new_token[0] == '\'') {
        tk->type = TOKEN_LITERAL_CHAR;
        tk->token_contents = new_token;
        return;
    }

    if(strchr(DIGITS, new_token[0]) != NULL) {
        tk->type = TOKEN_LITERAL_NUMERIC;
        tk->token_contents = new_token;
        return;
    }

    // If all else fails, it must be an identifier
    tk->type = TOKEN_IDENTIFIER;
    tk->token_contents = new_token;
}

int tokenize_and_lex(char* src_str, lex_tokens_t* list) {
    size_t line_counter = 1;
    size_t allocated_tokens = STARTING_TOKEN_LIST_LENGTH;

    list->tokens = malloc(sizeof(lex_token_t) * allocated_tokens);

    while(1) {
        // If current char is eof, we break
        if(*src_str == '\0') {
            break;
        }

        // If current char is start of the comment, we skip the comment
        if(*src_str == '#') {
            while(*src_str != '\n' && *src_str != '\0') src_str++;
            if(*src_str == '\n') line_counter++;
            continue;
        }

        // If current char is one of the whitespace characters, we skip it too
        if(strchr(WHITESPACE_CHARS, *src_str) != NULL) {
            if(*src_str == '\n') line_counter++;
            src_str++;
            continue;
        }

        // If neither of the above happened, it means that we found a new token
        // Check if we have allocated enough space, if not reallocate bigger buffer
        if(list->num_tokens + 1 > allocated_tokens) {
            void* new_buffer = malloc(sizeof(lex_token_t) * 2 * allocated_tokens);

            memcpy(new_buffer, list->tokens, allocated_tokens * sizeof(lex_token_t));
            free(list->tokens);
            list->tokens = new_buffer;
            allocated_tokens = 2 * allocated_tokens;
        }

        // Read the next token (it modifies the src_str iterator)
        char* new_token = _read_token(&src_str);

        // This frees the "new_token" buffer if it is not necessary to be kept,
        // otherwise it stores it in the list. No assumptions should be made about the new_token
        // after this point.
        _determine_token_type(new_token, list->tokens + list->num_tokens);

        // Also store reference to a source file line for error reporting
        list->tokens[list->num_tokens].line_ref = line_counter;

        list->num_tokens++;
    }
    return 0;
}

void tokens_clear(lex_tokens_t* list) {
    free(list->tokens);
}
