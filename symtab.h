#ifndef HASM_SYMBOL_TABLE_H
#define HASM_SYMBOL_TABLE_H


#include <stdint.h>

struct Symbol {
    char operand[50];
    uint16_t addr:16;
    struct Symbol *left, *right;
};/* Symbol tree */

/* predefined symbols */
extern struct Symbol predef_operands[];
/* insert new symbol to tree */
void insert_symtab(struct Symbol **node, const char *symbol, unsigned short addr);
/* search symbols */
struct Symbol *scan_symtab(struct Symbol *root, const char *symbol);
/* initialize tree via predefined operands */
void init_symtab(struct Symbol **sym_table);
/* cleanup tree recursively */
void cleanup_symtab(struct Symbol **tree);

#endif //HASM_SYMBOL_TABLE_H
