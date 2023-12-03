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

// list - macros which declare/implement a dynamic list for a particular type of element (emualating generics)

#ifndef _I_UTILS_LIST_H_
#define _I_UTILS_LIST_H_

#include <stddef.h>
#include <stdlib.h>

// This macro expands into a set of declarations for a dynamic list type
// It declares a structure for the list, with the name <type_prefix>_list_t, and
// a set of functions for initialization, destruction of dynamic state, as well as
// standard list operations. The structure contains a dynamic array of inner_type.
//
// Whenever a list of a certain type is needed, this macro should be used in the header file
// and the MAKE_LIST_IMPLEMENTATION should be used in the source file.
#define UTILS_LIST_MAKE_DECLARATION(type_prefix, inner_type) \
    struct type_prefix##_list_t {   \
        size_t num_elements;        \
        size_t alloc_elements;      \
        inner_type** arr;           \
    };                              \
    typedef struct type_prefix##_list_t type_prefix##_list_t;   \
    type_prefix##_list_t* type_prefix##_list_make();        \
    void type_prefix##_list_append(type_prefix##_list_t* l, inner_type* tk);   \
    void type_prefix##_list_destroy(type_prefix##_list_t* l);

// Implementation details:
// list_make() dynamically allocates a list and returns a pointer to it (has default_size capacity at beginning)
// each element is of type inner_type*
// the default grow strategy is to increase the capacity 2x
// list_append() takes a pointer to a inner_type structure and copies THE POINTER (not the data)
// Because of this, its up to the child structure to determine whether pointer points to static or dynamic memory
// elem_destroy_callback must be a name of the function that takes inner_type* as an argument, and whose
// role is to properly clean up each list element before the list itself may be freed.
// If no special care is needed for each element, elem_destroy_callback can be supplied with NULL.
// elem_destroy_callback's signature should be void func_name(inner_type*);
// elem_destroy_callback should call free() on the elem wherever it is needed 
// (might not always be, see types - builtin types are actually static structs, and type_destroy() reflects that).
#define UTILS_LIST_MAKE_IMPLEMENTATION(type_prefix, inner_type, default_size, elem_destroy_callback) \
    void __##type_prefix##_list_grow(type_prefix##_list_t* l) {     \
        l->alloc_elements = 2 * l->alloc_elements;                  \
        inner_type** new_arr = malloc(l->alloc_elements * sizeof(inner_type*));   \
        memcpy(new_arr, l->arr, l->num_elements * sizeof(inner_type*));  \
        free(l->arr);   \
        l->arr = new_arr;   \
    }   \
    \
    type_prefix##_list_t* type_prefix##_list_make() { \
        type_prefix##_list_t* l = malloc(sizeof(type_prefix##_list_t)); \
        l->num_elements = 0;  \
        l->alloc_elements = (size_t) default_size;  \
        l->arr = malloc(default_size * sizeof(inner_type*)); \
        return l; \
    }   \
    \
    void type_prefix##_list_append(type_prefix##_list_t* l, inner_type* elem) { \
        if(l->num_elements >= l->alloc_elements) {  \
            __##type_prefix##_list_grow(l);  \
        }   \
        l->arr[l->num_elements] = elem; \
        l->num_elements += 1;   \
    }   \
    void type_prefix##_list_destroy(type_prefix##_list_t* l) {  \
        if(l == NULL) return; \
        void (*cleanup_callback)(inner_type*) = elem_destroy_callback;  \
        if(cleanup_callback != NULL) {  \
            for(size_t i = 0; i < l->num_elements; i++) { \
                inner_type* elem = l->arr[i];    \
                cleanup_callback(elem); \
            };  \
        };  \
        free(l->arr);   \
        free(l);    \
    }

#define UTILS_LIST_GENERIC_LENGTH(list_ptr) (list_ptr->num_elements)
#define UTILS_LIST_GENERIC_GET(list_ptr, index) ((index < list_ptr->num_elements) ? list_ptr->arr[index] : NULL)

#endif
