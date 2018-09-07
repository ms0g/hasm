#ifndef HASM_LEXER_H
#define HASM_LEXER_H

#include <stdint.h>

enum tokens {
    LABEL,
    A_INS,
    C_INS,
    NUMBER
};

/* Assembler state */
enum state {
    pass1,
    pass2
};

/* C ins fields */
typedef struct {
    char *comp;
    char *dest;
    char *jmp;

} C_ins_t;


int tokenize(char *, char *, int *, C_ins_t *, int);

#endif //HASM_LEXER_H
