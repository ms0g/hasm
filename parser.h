#ifndef HASM_PARSER_H
#define HASM_PARSER_H

#include <stdio.h>
#include <stdint.h>
#include "symtab.h"

/* Int,out file */
FILE *intfp, *outfp;

/* extract operands, create intermediate file,update symbol table */
void init_analysis(FILE *srcfp, char *file_name, struct Symbol **sym_tbl);

/* process intermediate file via symbol table */
void init_synthesis(char *file_name, struct Symbol **sym_tbl);

#endif //HASM_PARSER_H
