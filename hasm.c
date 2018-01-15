#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <getopt.h>
#include "parser.h"
#include "lib/hasmlib.h"


FILE *src_file = NULL;

/* Symbol table */
struct Symbol *sym_tbl = NULL;
void cleanup();

int main(int argc, char *argv[]) {
    int opt;
    const char *usage = "Usage: ./hasm [file]";
    while ((opt = getopt(argc, argv, "h:")) != -1) {
        switch (opt) {
            case 'h':
                printf("%s\n", usage);
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s [file]\n",
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (argv[optind] == NULL || strlen(argv[optind]) == 0) {
        printf("%s\n", usage);
        return 0;
    }

    src_file = hfopen(argv[optind], "r");

    assert(src_file != NULL);

    // initializing symbol table
    init_symtab(&sym_tbl);

    // pass 1
    init_analysis(src_file, argv[optind], &sym_tbl);

    // pass 2
    init_synthesis(argv[optind], &sym_tbl);

    cleanup();

    return 0;
}

void cleanup() {
    cleanup_symtab(&sym_tbl);
    hfclose(src_file);
    hfclose(ifile);
    hfclose(ofile);
}
