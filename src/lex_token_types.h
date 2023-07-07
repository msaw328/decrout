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

// lex_token_types - Identification of text tokens in source code

#ifndef _I_LEX_TOKEN_TYPES_H_
#define _I_LEX_TOKEN_TYPES_H_

#include <stdint.h>

enum lex_token_type_t {
    TOKEN_RT = 0, // "rt"
    TOKEN_DECL, // "decl"
    TOKEN_RETURN, // "return"
    TOKEN_CONST, // "const"
    TOKEN_AT, // "@"
    TOKEN_DOLLAR, // "$"
    TOKEN_ASTERISK, // "*"
    TOKEN_SLASH, // "/"
    TOKEN_PLUS, // "+"
    TOKEN_MINUS, // "-"
    TOKEN_EQUAL, // "="
    TOKEN_TILDE, // "~"
    TOKEN_AMPERSAND, // "&"
    TOKEN_PIPE, // "|"
    TOKEN_UP_ARROW, // "^"
    TOKEN_EXCLAMATION, // "!"
    TOKEN_DOUBLE_AMPERSAND, // "&&"
    TOKEN_DOUBLE_PIPE, // "||"
    TOKEN_DOUBLE_EQUAL, // "=="
    TOKEN_TWO_TRIANGLES, // "<>"
    TOKEN_TRIANGLE_LEFT, // "<"
    TOKEN_TRIANGLE_RIGHT, // ">"
    TOKEN_TRIANGLE_EQUAL_LEFT, // "<="
    TOKEN_TRIANGLE_EQUAL_RIGHT, // ">="
    TOKEN_DOT, // "."
    TOKEN_BRACKET, // "{"
    TOKEN_END_BRACKET, // "}"
    TOKEN_SQUARE, // "["
    TOKEN_END_SQUARE, // "]"
    TOKEN_PAREN, // "("
    TOKEN_END_PAREN, // ")"
    TOKEN_COLON, // ":"
    TOKEN_SEMICOLON, // ";"
    TOKEN_COMMA, // ","

    TOKEN_LITERAL_STRING, // any string literal of type "..."
    TOKEN_LITERAL_NUMERIC, // any numeric literal like 1234 or 0x567
    TOKEN_LITERAL_CHAR, // any character literal like 'k' or '\x96'
    TOKEN_IDENTIFIER // any identifier - the most loose category of tokens
};
typedef enum lex_token_type_t lex_token_type_t;

#endif
