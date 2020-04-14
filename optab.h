#ifndef HASM_OPTAB_H
#define HASM_OPTAB_H
#include "utils.h"

enum op_type {
    hasm_comp,
    hasm_dest,
    hasm_jmp
};

/* Scan opcodes */
u16 scan_opc(const char *opcode, const enum op_type type);

#endif //HASM_OPTAB_H
