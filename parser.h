#ifndef HASM_PARSER_H
#define HASM_PARSER_H

#include <stdio.h>
#include <stdint.h>
#include "symtab.h"

/* extract operands, create intermediate file,update symbol table */
void init_analysis(FILE *infp, char *file_name, struct Symbol **sym_table);

/* process intermediate file via symbol table */
void init_synthesis(FILE *infp, FILE *outfp, struct Symbol **sym_table);

#endif //HASM_PARSER_H
