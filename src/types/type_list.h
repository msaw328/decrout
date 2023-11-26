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

#ifndef _I_TYPES_TYPE_LIST_H_
#define _I_TYPES_TYPE_LIST_H_

#include <stddef.h>

#include "types.h"
#include "utils/list.h"

// Declare a list of type_info_t, used in args of routine type_info_t
UTILS_LIST_MAKE_DECLARATION(type_info, struct type_info_t)

// Deallocate memory owned by the type_info structure
void type_destroy(struct type_info_t* type);

#endif
