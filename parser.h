#ifndef HASM_PARSER_H
#define HASM_PARSER_H

#include <stdio.h>
#include <stdint.h>
#include "symbol_table.h"

enum C_INS_FIELD {
    DEST,
    COMP,
    JMP
};

struct C_INS {
    const char *name;
    enum C_INS_FIELD type;
    unsigned short opcode:16;
}; /* Instructions table */


extern struct C_INS _instructions[]; /* opcode mnemonics */
/* operand extractions */
void preprocessor(FILE *src_file, char *file_name, struct Symbol **sym_tbl);

void processor(char *file_name, struct Symbol **sym_tbl);
unsigned short opcode_search(char* name,enum C_INS_FIELD type);

#endif //HASM_PARSER_H
