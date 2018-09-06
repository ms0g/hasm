#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <getopt.h>
#include "parser.h"
#include "utils.h"
#include "hdr.h"

#define ADD_SUFFIX(fname, suf) (strcat(fname, suf))

#define OUTFN(fname)            \
char *t = strchr(fname, '.');   \
*t = '\0';                      \
ADD_SUFFIX(fname, ".hex");

FILE *srcfp = NULL;

/* Symbol table */
struct Symbol *sym_tbl = NULL;

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
        srcfp = hasm_fopen(argv[optind], "r");
    } else {
        hasm_error("error: %s: No such file or directory\n", Fatal, argv[optind]);
    }

    // initializing symbol table
    init_symtab(&sym_tbl);

    // create intermediate file
    ADD_SUFFIX(argv[optind], ".int");

    // pass 1
    init_analysis(srcfp, argv[optind], &sym_tbl);

    char *file_name = strdup(argv[optind]);

    OUTFN(argv[optind])
    // output file
    outfp = hasm_fopen(argv[optind], "wb");
    write_hdr(outfp);

    // pass 2
    init_synthesis(file_name, &sym_tbl);

    free((void *)file_name);
    cleanup();

    return 0;
}

static void cleanup() {
    cleanup_symtab(&sym_tbl);
    hasm_fclose(srcfp);
    hasm_fclose(intfp);
    hasm_fclose(outfp);
}
