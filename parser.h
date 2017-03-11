#ifndef HASM_PARSER_H
#define HASM_PARSER_H

#include <stdio.h>
#include <stdint.h>
#include "symbol_table.h"

enum C_INS_FIELD {
    DEST,
    COMP,
    JMP
};/* C instruction fields */

struct C_INS {
    const char *name;
    enum C_INS_FIELD type;
    unsigned short opcode:16;
}; /* Instructions table */

/* opcode mnemonics */
extern struct C_INS _instructions[];

/* extract operands, create intermediate file,update symbol table */
void preprocessor(FILE *src_file, char *file_name, struct Symbol **sym_tbl);
/* process intermediate file via symbol table */
void processor(FILE *src_file, char *file_name, struct Symbol **sym_tbl);
/* search mnemonics */
unsigned short opcode_search(char* name, enum C_INS_FIELD type);

#endif //HASM_PARSER_H
