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
    char comp[2];
    char dest[2];
    char jmp[2];

} C_INS_t;


int tokenize(char *buf, char *token, int *tok_type, C_INS_t *c_inst, int state);

#endif //HASM_LEXER_H
