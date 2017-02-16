#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

struct Symbol predef_operands[] = {
        {"R0",     0x0000},
        {"R1",     0x0001},
        {"R2",     0x0002},
        {"R3",     0x0003},
        {"R4",     0x0004},
        {"R5",     0x0005},
        {"R6",     0x0006},
        {"R7",     0x0007},
        {"R8",     0x0008},
        {"R9",     0x0009},
        {"R10",    0x000A},
        {"R11",    0x000B},
        {"R12",    0x000C},
        {"R13",    0x000D},
        {"R14",    0x000E},
        {"R15",    0x000F},
        {"SP",     0x0000},
        {"LCL",    0x0001},
        {"ARG",    0x0002},
        {"THIS",   0x0003},
        {"THAT",   0x0004},
        {"SCREEN", 0x4000},
        {"KBD",    0x6000}
};

#define SIZE_OF_SYMBOLS (int)(sizeof(predef_operands)/sizeof(predef_operands[0]))

void init_sym_table(struct Symbol **sym_table) {
    for (int i = 0; i < SIZE_OF_SYMBOLS; ++i) {
        insert(sym_table,predef_operands[i].operand,predef_operands[i].bin);
    }

}


void insert(struct Symbol **node, const char *symbol, unsigned short bin) {
    struct Symbol *temp;
    if (*node == NULL) {
        temp = (struct Symbol *) malloc(sizeof(struct Symbol));
        if (temp == NULL) {
            fprintf(stderr, "Unable to allocate symbol");
            exit(1);
        }
        strcpy(temp->operand,symbol);
        temp->bin = bin;
        temp->left = temp->right = NULL;

        *node = temp;
    } else {
        if (strcmp(symbol, (*node)->operand) > 0)
            insert(&(*node)->right, symbol, bin);
        else
            insert(&(*node)->left, symbol, bin);
    }
}


struct Symbol *operand_search(struct Symbol *root, const char *symbol) {
    if (root == NULL || strcmp(symbol, root->operand) == 0) {
        return root;
    } else if (strcmp(symbol, root->operand) > 0) {
        return operand_search(root->right, symbol);
    } else
        return operand_search(root->left, symbol);
}


void clear(struct Symbol **tree) {
    if (*tree == NULL) return;
    clear(&(*tree)->right);
    clear(&(*tree)->left);
    free(*tree);
    *tree = NULL;
}
