#ifndef HASM_LEXER_H
#define HASM_LEXER_H

#include <stdint.h>
#include "utils.h"

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

/* C Instruction fields */
typedef struct {
    char *comp;
    char *dest;
    char *jmp;
    
} C_INS_t;


int tokenize(char *buf, char *token, int *tok_type, C_INS_t *c_inst, int state);

/* Getter Methods */
u16 c_inst_dest_get(C_INS_t *inst);
u16 c_inst_comp_get(C_INS_t *inst);
u16 c_inst_jmp_get(C_INS_t *inst);

/* Setter Methods */
void c_inst_dest_set(C_INS_t *inst, char *token);
void c_inst_comp_set(C_INS_t *inst, char *token);
void c_inst_jmp_set(C_INS_t *inst, char *token);

#endif //HASM_LEXER_H
