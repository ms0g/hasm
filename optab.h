#ifndef HASM_OPTAB_H
#define HASM_OPTAB_H

enum op_type {
    COMP,
    DEST,
    _JMP
};

/**
 * Scan opcodes
 */
uint16_t scan_opc(const char *opcode, const enum op_type type);

#endif //HASM_OPTAB_H
