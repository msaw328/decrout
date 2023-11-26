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

// type_list - List of types, used in routine args

#include "types.h"
#include "type_list.h"

// Make implementation of type_info_list_t (used in routine args)
UTILS_LIST_MAKE_IMPLEMENTATION(type_info, struct type_info_t, 8, type_destroy)

void type_destroy(type_info_t* type) {
    if(type == NULL || type->family == TYPE_FAMILY_BUILTIN) return;

    switch(type->family) {
        case TYPE_FAMILY_POINTER: {
            type_destroy(type->type_data.pointer.type);
            break;
        }

        case TYPE_FAMILY_ROUTINE: {
            type_info_list_destroy(type->type_data.routine.args);
            type_destroy(type->type_data.routine.return_type);
            break;
        }

        default:
            break;
    }

    free(type);

    return;
}
