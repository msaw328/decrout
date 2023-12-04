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

// args - General cli argument handling

#include "args.h"

#include "version.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

void _print_usage_and_exit() {
    puts("dcrtc - Decrout compiler");
    printf("Usage: dcrtc [-hvso] <input filename>\n");
    puts("\t-h\t\t- print this help and exit");
    puts("\t-v\t\t- print version information");
    puts("\t-s(0-1)\t\t- stage to output (default: last stage)");
    puts("\tstages in order: 0 - lexing, 1 - parsing");
    puts("\t-o <filename>\t- output filename to write to (default: stdout)");
    exit(0);
}

void _print_version_and_exit() {
    puts("dcrtc - Decrout compiler");
    puts("Copyright (C) 2023  Maciej Sawka <maciejsawka@gmail.com> <msaw328@kretes.xyz>");
    printf("Commit id: %s\nBuild date: %s\n\n", DCRTC_COMMIT_ID, DCRTC_BUILD_DATE);
    exit(0);
}

context_args_t* context_args_parse(int argc, char** argv) {
    context_args_t* args = malloc(sizeof(context_args_t));

    int end_of_options = 0;

    int output_stage_provided = 0;
    int output_file_provided = 0;

    // default values
    args->output_stage = STAGE_PARSER;
    args->output_file = stdout;
    args->input_file = NULL;

    // https://www.gnu.org/software/libc/manual/html_node/Using-Getopt.html
    // https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
    int c = 0;
    while((c = getopt(argc, argv, "vho:s:")) && end_of_options != 1) {
        switch(c) {
            // h - print help and exit
            case 'h': {
                free(args);
                _print_usage_and_exit();
                return NULL;
            }

            // h - print help and exit
            case 'v': {
                free(args);
                _print_version_and_exit();
                return NULL;
            }

            // o - output filename
            case 'o': {
                if(output_file_provided != 0) {
                    free(args);
                    fprintf(stderr, "[context] Error parsing arguments: duplicate '-o' option.\n");
                    return NULL;
                }

                args->output_file = fopen(optarg, "w");
                if(args->output_file == NULL) {
                    free(args);
                    fprintf(stderr, "[context] Error parsing arguments: cannot open file %s for writing.\n", optarg);
                    return NULL;
                }

                output_file_provided = 1;
                break;
            }

            // s - stage number to output
            case 's': {
                if(output_stage_provided != 0) {
                    free(args);
                    fprintf(stderr, "[context] Error parsing arguments: duplicate '-s' option.\n");
                    return NULL;
                }

                if(strlen(optarg) > 1) {
                    free(args);
                    fprintf(stderr, "[context] Error parsing arguments: stage numbers are one digit only\n");
                    return NULL;
                }

                // Since its a single digit, this is easy to convert to a number
                int stage_number = ((int) optarg[0]) - (int) '0';

                if(stage_number < (int) STAGE_FIRST || stage_number > (int) STAGE_LAST) {
                    free(args);
                    fprintf(stderr, "[context] Error parsing arguments: stage number invalid or out of range: %s\n", optarg);
                    return NULL;
                }

                args->output_stage = stage_number;
                output_stage_provided = 1;
                break;
            }

            default:
            case '?': {
                end_of_options = 1;
                break;
            }
        }
    }

    // If there is a remaining non-option arg, its an input file
    if(optind < argc) {
        args->input_file = fopen(argv[optind], "r+");
        if(args->input_file == NULL) {
            free(args);
            fprintf(stderr, "[context] Error parsing arguments: cannot open file %s for reading.\n", argv[optind]);
            return NULL;
        }
    } else {
        free(args);
        fprintf(stderr, "[context] Error parsing arguments: input file name was not provided\n");
        return NULL;
    }

    return args;
}
