#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <getopt.h>
#include "parser.h"

#define INT_FILE_NAME(s) strcat(s,".int")
#define REMOVE_EXT(s) {\
        char* t=strchr(s,'.');\
        *t='\0';\
        }
#define BIN_FILE(s) strcat(s,".o")


int main(int argc, char *argv[]) {
    int opt;
    FILE *src_file = NULL;
    char *file_name;
    struct Symbol* sym_tbl = NULL;

    const char* usage = "Usage: ./hasm [file]";
    while ((opt = getopt(argc, argv, "h:")) != -1)
    {
        switch (opt){
            case 'h':
                printf("%s\n", usage);
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s [file]\n",
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (argv[optind] == NULL || strlen(argv[optind]) == 0){
        printf("%s\n", usage);
        return 0;
    }
    else {
        file_name = argv[optind];
        src_file = fopen(argv[optind], "r");
    }
    assert(src_file != NULL);

    // initializing symbol table
    init_sym_table(&sym_tbl);

    INT_FILE_NAME(file_name);
    // extracting labels
    preprocessor(src_file, file_name, &sym_tbl);

    src_file = fopen(file_name, "r");
    REMOVE_EXT(file_name);
    BIN_FILE(file_name);

    // processing
    processor(src_file, file_name, &sym_tbl);

    clear(&sym_tbl);

    fclose(src_file);
    return 0;
}