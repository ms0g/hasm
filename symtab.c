#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
#include "hasmlib.h"

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

void init_symtab(struct Symbol **sym_table) {
    for (int i = 0; i < SIZE_OF_SYMBOLS; ++i) {
        insert_symtab(sym_table, predef_operands[i].operand, predef_operands[i].addr);
    }

}


void insert_symtab(struct Symbol **node, const char *symbol, unsigned short addr) {
    struct Symbol *temp;
    // create new node
    if (*node == NULL) {

        temp = hmalloc(sizeof(struct Symbol));
        strcpy(temp->operand,symbol);
        temp->addr = addr;
        temp->left = temp->right = NULL;

        *node = temp;
    } else {
        // determine where to insert symtab
        if (strcmp(symbol, (*node)->operand) > 0)
            insert_symtab(&(*node)->right, symbol, addr);
        else
            insert_symtab(&(*node)->left, symbol, addr);
    }
}


struct Symbol *scan_symtab(struct Symbol *root, const char *symbol) {
    if (root == NULL || strcmp(symbol, root->operand) == 0) {
        return root;
    } else if (strcmp(symbol, root->operand) > 0) {
        return scan_symtab(root->right, symbol);
    } else
        return scan_symtab(root->left, symbol);
}


void cleanup_symtab(struct Symbol **tree) {
    if (*tree == NULL) return;
    cleanup_symtab(&(*tree)->right);
    cleanup_symtab(&(*tree)->left);
    free(*tree);
    *tree = NULL;
}
