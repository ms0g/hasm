#ifndef HASM_SYMBOL_TABLE_H
#define HASM_SYMBOL_TABLE_H


struct Symbol {
    char operand[50];
    unsigned short bin:16;
    struct Symbol *left, *right;
};

extern struct Symbol predef_operands[];

void insert(struct Symbol **node, const char *symbol, unsigned short val);
struct Symbol *operand_search(struct Symbol *root, const char *symbol);
void init_sym_table(struct Symbol **sym_table);
void clear(struct Symbol **tree);

#endif //HASM_SYMBOL_TABLE_H
