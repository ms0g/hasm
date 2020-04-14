#ifndef HASM_SYMTAB_H
#define HASM_SYMTAB_H

#include <stdint.h>
#include "utils.h"

/* Symbol tree */
struct Symbol {
    char operand[50];
    u16 addr:16;
    struct Symbol *left, *right;
};



/* Insert new symbol to tree */
void insert_symtab(struct Symbol **node, const char *symbol, unsigned short addr);

/* Search symbols */
struct Symbol *scan_symtab(struct Symbol *root, const char *symbol);

/* Initialize the tree via predefined operands */
void init_symtab(struct Symbol **sym_table);

/* Cleanup tree recursively */
void cleanup_symtab(struct Symbol **sym_table);

#endif //HASM_SYMTAB_H
