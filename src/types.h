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

// types - Information about types

#ifndef _I_PARSER_H_
#define _I_PARSER_H_

#include <stddef.h>

#define TYPE_FAMILY_VOID 0
#define TYPE_FAMILY_NATIVE 1 // TODO: Add support for floating point
#define TYPE_FAMILY_POINTER 2
#define TYPE_FAMILY_ROUTINE 3
//#define TYPE_FAMILY_STRUCTURAL 4 TODO: Add support for structs
//#define TYPE_FAMILY_ALIAS 5 TODO: Add support for type aliasing

// Structure describing any basic native data type, including
// integers, void, bools, chars, floats (in the future)
// TODO: Maybe should add signedness etc?
struct type_info_native_t {
    char* name;
};
typedef struct type_info_native_t type_info_native_t;

// Structure describing a routine type (only usable through a pointer)
struct type_info_routine_t {
    size_t arg_count;
    struct type_info_t** arg_types;
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
        type_info_native_t native;
        type_info_routine_t routine;
        type_info_pointer_t pointer;
    } type_data;
};
typedef struct type_info_t type_info_t;

type_info_t* type_get_native_by_name(char* name);
type_info_t* type_make_pointer_to(type_info_t* type);
type_info_t* type_make_routine(size_t arg_c, type_info_t** args, type_info_t* ret);

int type_are_the_same(type_info_t* a, type_info_t* b);

char* type_to_string(type_info_t* type);

// Deallocated memory owned by the type_info structure
void type_destroy(type_info_t* type);

#endif
