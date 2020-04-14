#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <getopt.h>
#include "parser.h"
#include "utils.h"
#include "hdr.h"

#define ADD_SUFFIX(fname, suf)   \
char *t = strrchr(fname, '.');   \
*t = '\0';                       \
strcat(fname, suf);

/* Symbol table */
struct Symbol *sym_tbl = NULL;

/* Int,out file */
FILE *infp;
FILE *outfp;


static void cleanup();

int main(int argc, char *argv[]) {
    int opt;
    const char *usage = "Usage: ./hasm [file]";

    while ((opt = getopt(argc, argv, "h:")) != -1) {
        switch (opt) {
            case 'h':
                printf("%s\n", usage);
                break;
            default: /* '?' */
                hasm_error("Usage: %s [file]\n", Error, argv[0]);
        }
    }

    if (argv[optind] == NULL || strlen(argv[optind]) == 0) {
        hasm_error("error: %s: No such file or directory\n", Fatal, argv[optind]);
    }

    if (fd_isreg(argv[optind]) > 0) {
        infp = hasm_fopen(argv[optind], "r");
    } else {
        hasm_error("error: %s: No such file or directory\n", Fatal, argv[optind]);
    }

    // initializing symbol table
    init_symtab(&sym_tbl);

    // create intermediate file
    char *intfile = argv[optind];
    {
        ADD_SUFFIX(intfile, ".int");
        
    }

    // pass 1
    init_analysis(infp, intfile, &sym_tbl);
    
    fclose(infp);
    infp = hasm_fopen(intfile, "r");

    ADD_SUFFIX(intfile, ".hex")
    // output file
    outfp = hasm_fopen(intfile, "wb");
    write_hdr(outfp);

    // pass 2
    init_synthesis(infp, outfp, &sym_tbl);

    cleanup();

    return 0;
}

static void cleanup() {
    cleanup_symtab(&sym_tbl);
    hasm_fclose(outfp);
    hasm_fclose(infp);
}
