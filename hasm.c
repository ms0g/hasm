#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <getopt.h>
#include "parser.h"
#include "lib/hasmlib.h"


FILE *srcfp = NULL;

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
                logging("Usage: %s [file]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (argv[optind] == NULL || strlen(argv[optind]) == 0) {
        logging("%s\n", usage);
        return 0;
    }

    srcfp = hfopen(argv[optind], "r");

    assert(srcfp != NULL);

    // initializing symbol table
    init_symtab(&sym_tbl);

    // pass 1
    init_analysis(srcfp, argv[optind], &sym_tbl);

    // pass 2
    init_synthesis(argv[optind], &sym_tbl);

    cleanup();

    return 0;
}

void cleanup() {
    cleanup_symtab(&sym_tbl);
    hfclose(srcfp);
    hfclose(intfp);
    hfclose(outfp);
}
