#ifndef HASM_TOKENIZER_H
#define HASM_TOKENIZER_H

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

} C;

/* Check tokens */
static int is_label(const char *str);
static int is_AIns(const char *str);
static int is_CIns(const char *str);
int is_space(const char *str);
static int check_match(const char *str, const char *rgx);

/* Initialize tokenizing */
void init_tokenizing(const char *buf, char *token, int *tok_type, C *c_inst, int state);

#endif //HASM_TOKENIZER_H
