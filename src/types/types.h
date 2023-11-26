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

// types - Information about types, utils used for parsing etc.

// This file is used to represent and model metadata about types
// during parsing and in the resulting AST
//
// Types are represented using the type_info_t structure.
// The .family field is one of the TYPE_FAMILY_ macros below.
// Based on this, the appropriate field in the type_data union
// should be accessed. Each version of the union specifies data
// about the concrete type family. For a pointer, thats the type_info_t
// for the type being pointed to, for routine thats the types of the args
// and return type and for builtin types thats the name.
// The functions at the bottom of this file are meant to ease the process
// of creating type_infos and destroying them when they are not needed.
// All type_info_t are allocated cynamically, except for builtin types
// which are statically defined in one of the source codes.

#ifndef _I_TYPES_TYPES_H_
#define _I_TYPES_TYPES_H_

#include <stddef.h>
#include <string.h>

#include "type_list.h"

#define TYPE_FAMILY_VOID 0
#define TYPE_FAMILY_BUILTIN 1 // TODO: Add support for floating point
#define TYPE_FAMILY_POINTER 2
#define TYPE_FAMILY_ROUTINE 3
//#define TYPE_FAMILY_STRUCT 4 TODO: Add support for structs
//#define TYPE_FAMILY_ALIAS 5 TODO: Add support for type aliasing

// Structure describing any basic builtin data type, including
// integers, void, bools, chars, floats (in the future)
// TODO: Maybe should add signedness etc?
struct type_info_builtin_t {
    char* name;
};
typedef struct type_info_builtin_t type_info_builtin_t;

// Structure describing a routine type (only usable through a pointer)
struct type_info_routine_t {
    struct type_info_list_t* args;
    struct type_info_t* return_type;
};
typedef struct type_info_routine_t type_info_routine_t;

// Structure describing a pointer (it is defined by the type that it points to)
struct type_info_pointer_t {
    struct type_info_t* type;
};
typedef struct type_info_pointer_t type_info_pointer_t;

struct type_info_t {
    int family;
    size_t size; // in bytes
    union {
        type_info_builtin_t builtin;
        type_info_routine_t routine;
        type_info_pointer_t pointer;
    } type_data;
};
typedef struct type_info_t type_info_t;

// All the below functions take ownership of their arguments (Aside from char* name in get builtin) and later those are freed by type_destroy

// Returns pointer to a statically allocated structure of a builtin type (or NULL if not found)
type_info_t* type_get_builtin_by_name(char* name);

// Returns pointer to a dynamically allocated structure, which represents a pointer to type in argument
type_info_t* type_make_pointer_to(type_info_t* type);

// Returns pointer to a dynamically allocated structure, which represents a routine. args has to be dynamically allocated.
// It is te obligation of the caller to allocate it/create it.
type_info_t* type_make_routine(struct type_info_list_t* args, type_info_t* ret);

// Compare two types to make sure they are the same
int type_are_the_same(type_info_t* a, type_info_t* b);

char* type_to_string(type_info_t* type);

#endif
