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

#include "types.h"

#include <string.h>
#include <stdlib.h>

#include "utils/list.h"

// An array defining basic builtin types
static type_info_t _builtin_types[] = {
    {
        .family = TYPE_FAMILY_BUILTIN,
        .type_data.builtin = {
            .name = "void",
        }
    },
    {
        .family = TYPE_FAMILY_BUILTIN,
        .type_data.builtin = {
            .name = "bool",
        }
    },
    {
        .family = TYPE_FAMILY_BUILTIN,
        .type_data.builtin = {
            .name = "char",
        }
    },
    {
        .family = TYPE_FAMILY_BUILTIN,
        .type_data.builtin = {
            .name = "u8",
        }
    },
    {
        .family = TYPE_FAMILY_BUILTIN,
        .type_data.builtin = {
            .name = "i8",
        }
    },
    {
        .family = TYPE_FAMILY_BUILTIN,
        .type_data.builtin = {
            .name = "u16",
        }
    },
    {
        .family = TYPE_FAMILY_BUILTIN,
        .type_data.builtin = {
            .name = "i16",
        }
    },
    {
        .family = TYPE_FAMILY_BUILTIN,
        .type_data.builtin = {
            .name = "u32",
        }
    },
    {
        .family = TYPE_FAMILY_BUILTIN,
        .type_data.builtin = {
            .name = "i32",
        }
    },
    {
        .family = TYPE_FAMILY_BUILTIN,
        .type_data.builtin = {
            .name = "u64",
        }
    },
    {
        .family = TYPE_FAMILY_BUILTIN,
        .type_data.builtin = {
            .name = "i64",
        }
    },
};

#define BUILTIN_TYPES_NUM (sizeof(_builtin_types) / sizeof(_builtin_types[0]))

// returns static struct ptr
type_info_t* type_get_builtin_by_name(char* name) {
    for(size_t i = 0; i < BUILTIN_TYPES_NUM; i++) {
        type_info_t* ptr = _builtin_types + i;

        if(strcmp(ptr->type_data.builtin.name, name) == 0) {
            return ptr;
        };
    }

    return NULL;
}

// returns dynamic struct ptr
type_info_t* type_make_pointer_to(type_info_t* type) {
    type_info_t* new_type = malloc(sizeof(type_info_t));

    new_type->family = TYPE_FAMILY_POINTER;
    new_type->type_data.pointer.type = type;
    return new_type;
}

// returns dynamic struct ptr, args has to be dynamically allocated by caller
type_info_t* type_make_routine(type_info_list_t* args, type_info_t* ret) {
    type_info_t* new_type = malloc(sizeof(type_info_t));

    new_type->family = TYPE_FAMILY_ROUTINE;
    new_type->type_data.routine.args = args;
    new_type->type_data.routine.return_type = ret;

    return new_type;
}

int type_are_the_same(type_info_t* a, type_info_t* b) {
    if(a == NULL || b == NULL) return 0;

    if(a->family != b->family) return 0;

    switch(a->family) {
        case TYPE_FAMILY_BUILTIN: {
            //return strcmp(a->type_data.native.name, b->type_data.native.name) == 0; // <- this should be used if we copy builtin data type structures
            return a == b; // If we dont copy, but rather return pointers to static structures, then it should be enough to compare pointers
        }

        case TYPE_FAMILY_POINTER: {
            return type_are_the_same(a->type_data.pointer.type, a->type_data.pointer.type);
        }

        case TYPE_FAMILY_ROUTINE: {
            type_info_routine_t* a_rt = &(a->type_data.routine);
            type_info_routine_t* b_rt = &(b->type_data.routine);

            int a_args_empty = (a_rt->args == NULL) || (UTILS_LIST_GENERIC_LENGTH(a_rt->args) == 0);
            int b_args_empty = (b_rt->args == NULL) || (UTILS_LIST_GENERIC_LENGTH(b_rt->args) == 0);
            if(a_args_empty && b_args_empty) return 1;

            if(a_args_empty) return 0;
            if(b_args_empty) return 0;

            if(UTILS_LIST_GENERIC_LENGTH(a_rt->args) != UTILS_LIST_GENERIC_LENGTH(b_rt->args)) return 0;

            for(size_t i = 0; i < UTILS_LIST_GENERIC_LENGTH(a_rt->args); i++) {
                if(!type_are_the_same(UTILS_LIST_GENERIC_GET(a_rt->args, i), UTILS_LIST_GENERIC_GET(a_rt->args, i))) return 0;
            }

            return a_rt->return_type == b_rt->return_type;
        }

        default:
            return 0;
    }
}

// Walks through the type tree and builds a string buffer
char* type_to_string(type_info_t* type) {
    if(type == NULL) return NULL;

    switch(type->family) {
        case TYPE_FAMILY_BUILTIN: {
            char* name = malloc(strlen(type->type_data.builtin.name) + 1);
            strcpy(name, type->type_data.builtin.name);
            return name;
        }

        case TYPE_FAMILY_POINTER: {
            char* inner_type = type_to_string(type->type_data.pointer.type);
            char* name = malloc(strlen(inner_type) + 2);

            strcpy(name + 1, inner_type);
            name[0] = '>';
            name[1 + strlen(inner_type)] = '\0';

            free(inner_type);
            return name;
        }

        case TYPE_FAMILY_ROUTINE: {
            // TODO: This traverses the arg list twice, maybe its not needed?
            type_info_routine_t* rt = &(type->type_data.routine);

            size_t args_string_length = 0;
            if(rt->args != NULL) {
                args_string_length = (UTILS_LIST_GENERIC_LENGTH(rt->args) - 1) * 2; // In the arg list, args are separated by ", "

                for(size_t i = 0; i < UTILS_LIST_GENERIC_LENGTH(rt->args); i++) {
                    char* arg_type_name = type_to_string(UTILS_LIST_GENERIC_GET(rt->args, i));
                    args_string_length += strlen(arg_type_name);
                    free(arg_type_name);
                }
            }

            char* return_type_str = type_to_string(rt->return_type);
            size_t ret_type_len = strlen(return_type_str);
            free(return_type_str);

            // 4 for "rt [ ", then args string length, then 4 for " ]: " then length of the return type + 1 for null byte
            char* name = malloc(10 + args_string_length + ret_type_len);

            size_t offset = 0;

            memcpy(name + offset, "rt ", 3);
            offset += 3;

            if(rt->args != NULL) {
                name[offset] = '[';
                offset++;

                for(size_t i = 0; i < UTILS_LIST_GENERIC_LENGTH(rt->args); i++) {
                    char* arg_type_name = type_to_string(UTILS_LIST_GENERIC_GET(rt->args, i));

                    strcpy(name + offset, arg_type_name);
                    offset += strlen(arg_type_name);

                    if(i != UTILS_LIST_GENERIC_LENGTH(rt->args) - 1) {
                        memcpy(name + offset, ", ", 2);
                        offset += 2;
                    }

                    free(arg_type_name);
                }

                name[offset] = ']';
                offset++;
            }

            memcpy(name + offset, ": ", 2);
            offset += 2;

            char* return_type_name = type_to_string(rt->return_type);
            strcpy(name + offset, return_type_name);
            free(return_type_name);

            return name;
        }

        default:
            return NULL;
    }
}

