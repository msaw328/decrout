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

// fileread - Wrappers for system file IO

#include "fileread.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

char* read_source_file(char* filename) {
    struct stat statbuf = { 0 };
    stat(filename, &statbuf);

    size_t filesize = statbuf.st_size;
    char* filecontents = malloc(filesize + 1);

    FILE* infile = fopen(filename, "r");
    fread(filecontents, 1, filesize, infile);
    fclose(infile);
    filecontents[filesize] = '\0';

    return filecontents;
}
