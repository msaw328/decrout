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

#include "types.h"

#include <string.h>
#include <stdlib.h>

// An array defining basic native types
static type_info_t _native_types[] = {
    {
        .family = TYPE_FAMILY_NATIVE,
        .type_data.native = {
            .name = "void",
        }
    },
    {
        .family = TYPE_FAMILY_NATIVE,
        .type_data.native = {
            .name = "bool",
        }
    },
    {
        .family = TYPE_FAMILY_NATIVE,
        .type_data.native = {
            .name = "char",
        }
    },
    {
        .family = TYPE_FAMILY_NATIVE,
        .type_data.native = {
            .name = "u8",
        }
    },
    {
        .family = TYPE_FAMILY_NATIVE,
        .type_data.native = {
            .name = "i8",
        }
    },
    {
        .family = TYPE_FAMILY_NATIVE,
        .type_data.native = {
            .name = "u16",
        }
    },
    {
        .family = TYPE_FAMILY_NATIVE,
        .type_data.native = {
            .name = "i16",
        }
    },
    {
        .family = TYPE_FAMILY_NATIVE,
        .type_data.native = {
            .name = "u32",
        }
    },
    {
        .family = TYPE_FAMILY_NATIVE,
        .type_data.native = {
            .name = "i32",
        }
    },
    {
        .family = TYPE_FAMILY_NATIVE,
        .type_data.native = {
            .name = "u64",
        }
    },
    {
        .family = TYPE_FAMILY_NATIVE,
        .type_data.native = {
            .name = "i64",
        }
    },
};

#define NATIVE_TYPES_NUM (sizeof(_native_types) / sizeof(_native_types[0]))

type_info_t* type_get_native_by_name(char* name) {
    for(size_t i = 0; i < NATIVE_TYPES_NUM; i++) {
        type_info_t* ptr = _native_types + i;

        if(strcmp(ptr->type_data.native.name, name) == 0) {
            return ptr;
        };
    }

    return NULL;
}

type_info_t* type_make_pointer_to(type_info_t* type) {
    type_info_t* new_type = malloc(sizeof(type_info_t));

    new_type->family = TYPE_FAMILY_POINTER;
    new_type->type_data.pointer.type = type;
    return new_type;
}

type_info_t* type_make_routine(size_t arg_c, type_info_t** args, type_info_t* ret) {
    type_info_t* new_type = malloc(sizeof(type_info_t));

    new_type->family = TYPE_FAMILY_ROUTINE;
    new_type->type_data.routine.arg_count = arg_c;
    new_type->type_data.routine.arg_types = args;
    new_type->type_data.routine.return_type = ret;

    return new_type;
}

int type_are_the_same(type_info_t* a, type_info_t* b) {
    if(a == NULL || b == NULL) return 0;

    if(a->family != b->family) return 0;

    switch(a->family) {
        case TYPE_FAMILY_NATIVE: {
            //return strcmp(a->type_data.native.name, b->type_data.native.name) == 0; // <- this should be used if we copy native data type structures
            return a == b; // If we dont copy, but rather return pointers to static structures, then it should be enough to compare pointers
        }

        case TYPE_FAMILY_POINTER: {
            return type_are_the_same(a->type_data.pointer.type, a->type_data.pointer.type);
        }

        case TYPE_FAMILY_ROUTINE: {
            if(a->type_data.routine.arg_count != b->type_data.routine.arg_count) return 0;

            for(size_t i = 0; i < a->type_data.routine.arg_count; i++) {
                if(!type_are_the_same(a->type_data.routine.arg_types[i], b->type_data.routine.arg_types[i])) return 0;
            }

            return a->type_data.routine.return_type == b->type_data.routine.return_type;
        }

        default:
            return 0;
    }
}

char* type_to_string(type_info_t* type) {
    if(type == NULL) return NULL;

    switch(type->family) {
        case TYPE_FAMILY_NATIVE: {
            char* name = malloc(strlen(type->type_data.native.name) + 1);
            strcpy(name, type->type_data.native.name);
            return name;
        }

        case TYPE_FAMILY_POINTER: {
            char* inner_type = type_to_string(type->type_data.pointer.type);
            char* name = NULL;

            if(type->type_data.pointer.type->family == TYPE_FAMILY_ROUTINE) { // If its a pointer to a routine, its best to also put it into ()s
                name = malloc(strlen(inner_type) + 4); // null byte + () + > = 4

                name[0] = '(';
                memcpy(name + 1, inner_type, strlen(inner_type));
                name[1 + strlen(inner_type)] = ')';
                name[2 + strlen(inner_type)] = '>';
                name[3 + strlen(inner_type)] = '\0';
            } else {
                name = malloc(strlen(inner_type) + 2);

                strcpy(name, inner_type);
                name[strlen(inner_type)] = '>';
                name[1 + strlen(inner_type)] = '\0';
            }

            free(inner_type);
            return name;
        }

        case TYPE_FAMILY_ROUTINE: {
            size_t args_string_length = (type->type_data.routine.arg_count - 1) * 2; // In the arg list, args are separated by ", "
            for(size_t i = 0; i < type->type_data.routine.arg_count; i++) {
                char* arg_type_name = type_to_string(type->type_data.routine.arg_types[i]);
                args_string_length += strlen(arg_type_name);
                free(arg_type_name);
            }

            char* return_type_str = type_to_string(type->type_data.routine.return_type);
            size_t ret_type_len = strlen(return_type_str);
            free(return_type_str);

            // 4 for "rt [ ", then args string length, then 4 for " ]: " then length of the return type + 1 for null byte
            char* name = malloc(10 + args_string_length + ret_type_len);

            size_t offset = 0;

            memcpy(name + offset, "rt [ ", 5);
            offset += 5;

            for(size_t i = 0; i < type->type_data.routine.arg_count; i++) {
                char* arg_type_name = type_to_string(type->type_data.routine.arg_types[i]);

                strcpy(name + offset, arg_type_name);
                offset += strlen(arg_type_name);

                if(i != type->type_data.routine.arg_count - 1) {
                    memcpy(name + offset, ", ", 2);
                    offset += 2;
                }

                free(arg_type_name);
            }

            memcpy(name + offset, " ]: ", 4);
            offset += 4;

            char* return_type_name = type_to_string(type->type_data.routine.return_type);
            strcpy(name + offset, return_type_name);
            free(return_type_name);

            return name;
        }

        default:
            return NULL;
    }
}

void type_destroy(type_info_t* type) {
    if(type == NULL || type->family == TYPE_FAMILY_NATIVE) return;

    switch(type->family) {
        case TYPE_FAMILY_POINTER: {
            type_destroy(type->type_data.pointer.type);
            break;
        }

        case TYPE_FAMILY_ROUTINE: {
            for(size_t i = 0; i < type->type_data.routine.arg_count; i++) {
                type_destroy(type->type_data.routine.arg_types[i]);
            }

            type_destroy(type->type_data.routine.return_type);
            break;
        }

        default:
            break;
    }

    free(type);

    return;
}
