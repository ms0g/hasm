#ifndef HASM_SYMBOL_TABLE_H
#define HASM_SYMBOL_TABLE_H


struct Symbol {
    char operand[50];
    unsigned short bin:16;
    struct Symbol *left, *right;
};/* Symbol tree */

/* predefined symbols */
extern struct Symbol predef_operands[];
/* insert new symbol to tree */
void insert(struct Symbol **node, const char *symbol, unsigned short val);
/* search symbols */
struct Symbol *operand_search(struct Symbol *root, const char *symbol);
/* initialize tree via predefined operands */
void init_sym_table(struct Symbol **sym_table);
/* clear tree recursively */
void clear(struct Symbol **tree);

#endif //HASM_SYMBOL_TABLE_H
