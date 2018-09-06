#ifndef HASM_PARSER_H
#define HASM_PARSER_H

#include <stdio.h>
#include <stdint.h>
#include "symtab.h"

/* Int,out file */
FILE *intfp, *outfp;

/* extract operands, create intermediate file,update symbol table */
void init_analysis(FILE *, char *, struct Symbol **);

/* process intermediate file via symbol table */
void init_synthesis(char *, struct Symbol **);

#endif //HASM_PARSER_H
