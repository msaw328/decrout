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

// parse_types - Parsing of type decalrations

#ifndef _I_PARSER_PARSE_TYPES_H_
#define _I_PARSER_PARSE_TYPES_H_

#include "types/types.h"
#include "lexer/token_list.h"

// Assumes iterator points to the first token of a type
// Consumes tokens until type is fully described
// Returns NULL if error, or new declaration if ok
type_info_t* parser_parse_type(lexer_token_iterator_t* iter);

#endif
