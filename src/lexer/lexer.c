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
#include <ctype.h>
#include <sys/types.h> // ssize_t

// TODO: Handle CRLF newlines? Like, in the entire file

// All the non-dynamic tokens, which have predefined char sequences
// Some are keywords (require whitespace around them) and others are not (do not require)
static char* _static_tokens[] = {
#define KEYWORDS_FIRST TOKEN_RT
    [TOKEN_RT] = "rt",
    [TOKEN_DECL] = "decl",
    [TOKEN_RETURN] = "return",
    [TOKEN_CONST] = "const",
#define KEYWORDS_LAST TOKEN_CONST

#define NON_KEYWORDS_FIRST TOKEN_AT
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
#define NON_KEYWORDS_LAST TOKEN_COMMA
};

#define STARTING_TOKEN_ALLOC_LENGTH 8
#define WHITESPACE_CHARS "\t\r\n "

// Digit sets for various numeric literals types
#define DIGITS "0123456789abcdefABCDEF"

// Check whether char is valid in identifiers
// 1 for true, 0 for false
// ATM the rules are: ascii alphanumeric characters + underscore (might change)
int _is_valid_in_identifiers(char c) {
    if(c >= 'a' && c <= 'z') return 1;

    if(c >= 'A' && c <= 'Z') return 1;

    if(c >= '0' && c <= '9') return 1;

    if(c == '_') return 1;

    return 0;
}

// First byte after a dynamic or keyword token should either be
// - whitespace
// - eof
// - beginning of a non-keyword token
//
// this function checks if the char at src_str meets that criteria
// Non-keyword static tokens do not have that requirement and do not necessitate the use of that function
int _is_proper_termination(char* src_str) {
    // Determine if the token was properly closed: either by whitespace, end of file...
    if(strchr(WHITESPACE_CHARS, *src_str) != NULL || *src_str == '\0') {
        return 1;
    }

    // ...or a non-keyword token (these are all valid options)
    for(size_t idx = NON_KEYWORDS_FIRST; idx <= NON_KEYWORDS_LAST; idx++) {
        char* static_token = _static_tokens[idx];

        if(strncmp(static_token, src_str, strlen(static_token)) == 0) {
            return 1;
        }
    }

    return 0;
}

// Determine how long is the token at src_str, and also it's type
// Return -1 instead, if weird stuff happens (for example, string literal without terminating " before newline/eof)
ssize_t _determine_token_length_and_type(char* src_str, lexer_token_type_t* token_type) {
    size_t length = 0;

    // First case: string/char literal
    if(src_str[length] == '"' || src_str[length] == '\'') {
        char literal_type = src_str[length]; // Literal type is either ' or "

        length++;

        // Consume another char if its not the end of the literal, or if the literal is preceded by '\' escaping it
        while(src_str[length] != literal_type) {
            // Check to make sure its not end of file, newline, tab
            // If there are other characters which have no right to be in a string literal
            // they should go here
            if(strchr("\0\n\t", src_str[length]) != NULL) return -1;

            length++;
            
            // In order to not break the loop we must consume two chars at once in case of \" and \'
            // Also, in order to handle cases such as "\\"
            if(src_str[length - 1] == '\\' && (src_str[length] == '\\' || src_str[length] == literal_type)) length++;
        }

        // Gotta also include the ending quote/apostrophe
        length++;

        // Check if the dynamic token was properly terminated
        if(_is_proper_termination(src_str + length) == 0) return -1;

        if(literal_type == '"') {
            *token_type = TOKEN_LITERAL_STRING;
        } else {
            *token_type = TOKEN_LITERAL_CHAR;
        }

        return (ssize_t) length;
    }

    // Second case: numeric literal
    // It has to start with one of the decimal digits, so first 10 characters of the DIGITS string
    if(memchr(DIGITS, *src_str, 10) != NULL) {
        // Determine the number of digits to look for (aka the base of hte literal: 2, 8, 10 or 16)
        size_t digits_len = 0;

        // If it starts with 0 it may be octal (0) bin (0b) or hex (0x)
        if(src_str[length] != '0') {
            digits_len = 10; // Decimal
            *token_type = TOKEN_LITERAL_NUMERIC_DEC;

        } else if(src_str[length + 1] == 'x' || src_str[length + 1] == 'X') {
            digits_len = sizeof(DIGITS) - 1; // Hex includes all digits
            *token_type = TOKEN_LITERAL_NUMERIC_HEX;
            length += 2;

        } else if(src_str[length + 1] == 'b' || src_str[length + 1] == 'B') {
            digits_len = 2; // Binary
            *token_type = TOKEN_LITERAL_NUMERIC_BIN;
            length += 2;

        } else {
            digits_len = 8; // Octal
            *token_type = TOKEN_LITERAL_NUMERIC_OCT;
            length++;
        }

        // Now just consume chars as long as they are in the digit range
        while(memchr(DIGITS, src_str[length], digits_len) != NULL) {
            length++;
        }

        // Check if the dynamic token was properly terminated
        if(_is_proper_termination(src_str + length) == 0) return -1;

        return length;
    }

    // Third case: Static token
    // Just check string equality, also not necessary to check proper termination for static non-keyword tokens
    size_t longest_match_idx = 0xFFFFFFFF;
    size_t longest_match_length = 0;
    for(size_t idx = KEYWORDS_FIRST; idx <= NON_KEYWORDS_LAST; idx++) {
        char* static_token = _static_tokens[idx];
        size_t tk_len = strlen(static_token);

        if(strncmp(static_token, src_str, tk_len) == 0) {
            // Keywords require proper termination
            if(idx <= KEYWORDS_LAST) {
                if(_is_proper_termination(src_str + tk_len) == 0) continue;
            }

            if(tk_len > longest_match_length) {
                longest_match_idx = idx;
                longest_match_length = tk_len;
            }
        }
    }

    if(longest_match_idx != 0xFFFFFFFF && longest_match_length > 0) {
        *token_type = longest_match_idx;
        return longest_match_length;
    }

    // Last resort: Identifier
    // Consume chars as they go, as long as they meet vague criteria for identifiers which at this point are:
    // alphanumeric characters + underscore

    // If not valid, return -1
    if(_is_valid_in_identifiers(src_str[length]) == 0) {
        return -1;
    }

    *token_type = TOKEN_IDENTIFIER;
    while(_is_valid_in_identifiers(src_str[length])) {
        length++;
    }

    // Check if the dynamic token was properly terminated
    if(_is_proper_termination(src_str + length) == 0) return -1;

    return length;
}

char* _read_next_token(char* src_str, size_t* line_counter_ptr, size_t* char_counter_ptr, lexer_token_t* next_token) {
    ssize_t token_length_try = _determine_token_length_and_type(src_str, &(next_token->type));
    if(token_length_try < 0) {
        fprintf(stderr, "[lexer] Error at line %zu char %zu: Can't determine token length and/or type\n", *line_counter_ptr, *char_counter_ptr);
        return NULL;
    }

    size_t token_length = (size_t) token_length_try;

    next_token->line_ref = *line_counter_ptr;
    next_token->char_ref = *char_counter_ptr;

    *char_counter_ptr += token_length;

    if(TOKEN_TYPE_IS_DYNAMIC(next_token->type)) {
        next_token->contents = malloc(token_length + 1);
        memcpy(next_token->contents, src_str, token_length);
        next_token->contents[token_length] = '\0';
    } else {
        next_token->contents = NULL;
    }

    return src_str + token_length;
}

char* _skip_to_next_token(char* src_str, size_t* line_counter_ptr, size_t* char_counter_ptr) {
    while(1) {
        char c = *src_str;

        // Handle EOF
        if(c == '\0') {
            return NULL;
        }

        // If current char is start of the comment, we skip the comment
        if(c == '#') {
            // Eat all the chars until we encounter a newline
            // Or end of file
            while(*src_str != '\n' && *src_str != '\0') src_str++;

            // The src_str now points at either \n or eof, let the next iteration
            // of the loop process it
            continue;
        }


        // If current char is one of the whitespace characters, we skip it too
        if(strchr(WHITESPACE_CHARS, c) != NULL) {
            // And set counters properly
            if(c == '\n') {
                (*line_counter_ptr)++;
                (*char_counter_ptr) = 1;
            } else {
                (*char_counter_ptr)++;
            }

            src_str++;
            continue;
        }

        // If nothing from the above happened, it means we have the next token
        return src_str;
    }
}

int lexer_process_source_code(char* src_str, lexer_token_list_t* list) {
    size_t line_counter = 1;
    size_t char_counter = 1;

    // Temporary buffer for token data, in the end the list will own the data
    lexer_token_t next_token = { 0 };

    while(1) {
        // Skip all the whitespace, comments etc
        // Return the address of first byte of the next token
        src_str = _skip_to_next_token(src_str, &line_counter, &char_counter);

        // If NULL is returned it means there is no more tokens, return
        if(src_str == NULL) break;

        // Otherwise return the next token - read the chars, detemrine type and fill struct
        // Return the address of first byte after the last char of the processed token
        src_str = _read_next_token(src_str, &line_counter, &char_counter, &next_token);

        // If NULL is returned it means there was a problem reading a token
        // (Possibly string without an ending " or something similar)
        if(src_str == NULL) {
            fprintf(stderr, "[lexer] Error at line %zu char %zu: Can't read token\n", line_counter, char_counter);
            return 1;
        }

        // If all is good, append the new token to the list
        lexer_token_list_append(list, &next_token);
    }

    return 0;
}
