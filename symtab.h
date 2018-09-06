#ifndef HASM_SYMBOL_TABLE_H
#define HASM_SYMBOL_TABLE_H

#include <stdint.h>


/* Symbol tree */
struct Symbol {
    char operand[50];
    uint16_t addr:16;
    struct Symbol *left, *right;
};



/* Insert new symbol to tree */
void insert_symtab(struct Symbol **, const char *, unsigned short);

/* Search symbols */
struct Symbol *scan_symtab(struct Symbol *, const char *);

/* Initialize the tree via predefined operands */
void init_symtab(struct Symbol **);

/* Cleanup tree recursively */
void cleanup_symtab(struct Symbol **);

#endif //HASM_SYMBOL_TABLE_H
