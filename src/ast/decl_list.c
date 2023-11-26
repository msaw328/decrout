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

// decl_list - list of decalrations

#include "decl_list.h"

#include <stddef.h>
#include <stdlib.h>

#include "utils/list.h"
#include "types/types.h"

// TODO: Once ast_decl_t contains dynamic data (list of expr) uncomment this and implement it
// void ast_decl_destroy(ast_decl_t* decl);

void ast_decl_destroy(ast_decl_t* decl) {
    type_destroy(decl->type);
    free(decl->symbol);
}

UTILS_LIST_MAKE_IMPLEMENTATION(ast_decl, struct ast_decl_t, 8, ast_decl_destroy)
