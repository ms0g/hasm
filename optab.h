#ifndef HASM_OPTAB_H
#define HASM_OPTAB_H

enum op_type {
    hasm_comp,
    hasm_dest,
    hasm_jmp
};

/* Scan opcodes */
uint16_t scan_opc(const char *, const enum op_type);

#endif //HASM_OPTAB_H
