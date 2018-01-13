#ifndef HASM_OPTAB_H
#define HASM_OPTAB_H

enum op_type{
    COMP,
    DEST,
    _JMP
};

/**
 * scan opcodes
 */
uint16_t scan_opc(char *opcode, enum op_type type);
#endif //HASM_OPTAB_H
