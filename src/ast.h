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

// ast - AST structures, used during parsing

#ifndef _I_AST_H_
#define _I_AST_H_

#include <stddef.h>

#include "types.h"

// All types of operations, such as + - * / @ $ etc
enum ast_expression_operations_t {
    AST_EXPR_OP_ADD = 0x0,
    AST_EXPR_OP_MUL,
    AST_EXPR_OP_SUB,
    AST_EXPR_OP_DIV,
    AST_EXPR_OP_DEREF,
    AST_EXPR_OP_PTR,
    AST_EXPR_OP_DOT,
    AST_EXPR_OP_BIN_NOT,
    AST_EXPR_OP_BIN_AND,
    AST_EXPR_OP_BIN_OR,
    AST_EXPR_OP_BIN_XOR,
    AST_EXPR_OP_LOG_NOT,
    AST_EXPR_OP_LOG_AND,
    AST_EXPR_OP_LOG_OR,
    AST_EXPR_OP_EQ,
    AST_EXPR_OP_NOT_EQ,
    AST_EXPR_OP_LT,
    AST_EXPR_OP_GT,
    AST_EXPR_OP_LT_OR_EQ,
    AST_EXPR_OP_GT_OR_EQ,
    AST_EXPR_OP_ASSIGN, // the "=" operation, which asides form assigning to the symbol also evaluates to the value
};

// A node which describes an operation on up to two operands
// for instance (a + b), (b * 4) etc
struct ast_operation_t {
    int op;
    struct ast_expression_t* left;
    struct ast_expression_t* right;
};
typedef struct ast_operation_t ast_operation_t;

// A node which describes a definition of a routine
// such expression would evaluate to an address of said routine
// and have a side effect of allocating space and generating code
// for said routine in the program memory
// example: rt [ a: u32 ]: u32 { return a + 5; }
struct ast_routine_def_t {
    size_t num_args;
    struct ast_declaration_t** args;
    type_info_t* return_type;

    size_t statements_num;
    struct ast_statement_t** body;
};
typedef struct ast_routine_def_t ast_routine_def_t;

// A string, char or numeric literal
#define AST_LITERAL_TYPE_STR 0x1
#define AST_LITERAL_TYPE_CHAR 0x2
#define AST_LITERAL_TYPE_NUM 0x3
struct ast_literal_t {
    int type;
    char* contents;
};
typedef struct ast_literal_t ast_literal_t;

// An expression which may be of multiple types
#define AST_EXPR_TYPE_OP 0x1
#define AST_EXPR_TYPE_RT 0X2
#define AST_EXPR_TYPE_LITERAL 0x3
#define AST_EXPR_TYPE_SYM 0x4
struct ast_expression_t {
    int type;
    union {
        ast_operation_t operation;
        ast_routine_def_t routine;
        ast_literal_t literal;
        char* symbol;
    } data;
};
typedef struct ast_expression_t ast_expression_t;

// A node which represents a declaration using either decl keyword or const
struct ast_declaration_t {
    int is_const;
    char* symbol;
    type_info_t* type;
    struct ast_expression_t* value;
};
typedef struct ast_declaration_t ast_declaration_t;

// A statement may be either a declaration or
// an expression
#define AST_STATEMENT_TYPE_DECL 0x1
#define AST_STATEMENT_TYPE_EXPR 0x2
struct ast_statement_t {
    int type;
    union {
        ast_declaration_t declaration;
        ast_expression_t expression;
    }
}

// Topmost structure of the AST, containing the global scope
// The global scope may contain only declarations!
struct ast_t {
    size_t num_declarations;
    struct ast_declaration_t** declarations;
};
typedef struct ast_t ast_t;

#endif
