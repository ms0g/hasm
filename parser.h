#ifndef HASM_PARSER_H
#define HASM_PARSER_H

#include <stdio.h>
#include <stdint.h>
#include "symtab.h"

/* extract operands, create intermediate file,update symbol table */
void init_analysis(FILE *src_file, char *file_name, struct Symbol **sym_tbl);
/* process intermediate file via symbol table */
void init_synthesis(char *fname, struct Symbol **sym_tbl);
void write_ofile(FILE *ofile);
#endif //HASM_PARSER_H
