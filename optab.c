#include <stdint.h>
#include <string.h>
#include "optab.h"
#include "hopcodes.h"


u16 scan_opc(const char *opcode, const enum op_type type) {
    if (opcode == NULL) {
        return 0;
    }
    if (STR_CMP(opcode, "0", 1))
        return COMP_ZERO;
    else if (STR_CMP(opcode, "1", 1))
        return COMP_ONE;
    else if (STR_CMP(opcode, "-1", 1))
        return COMP_MINUS_1;
    else if (STR_CMP(opcode, "D", 1)) {
        switch (type) {
            case hasm_comp:
                return COMP_D;
            case hasm_dest:
                return DEST_D;
            default:
                break;
        }
    } else if (STR_CMP(opcode, "A", 1)) {
        switch (type) {
            case hasm_comp:
                return COMP_A;
            case hasm_dest:
                return DEST_A;
            default:
                break;
        }
    } else if (STR_CMP(opcode, "!D", 2))
        return COMP_NOT_D;
    else if (STR_CMP(opcode, "!A", 2))
        return COMP_NOT_A;
    else if (STR_CMP(opcode, "-D", 2))
        return COMP_MINUS_D;
    else if (STR_CMP(opcode, "-A", 2))
        return COMP_MINUS_A;
    else if (STR_CMP(opcode, "D+1", 3))
        return COMP_D_PLUS_1;
    else if (STR_CMP(opcode, "A+1", 3))
        return COMP_A_PLUS_1;
    else if (STR_CMP(opcode, "D-1", 3))
        return COMP_D_MINUS_1;
    else if (STR_CMP(opcode, "A-1", 3))
        return COMP_A_MINUS_1;
    else if (STR_CMP(opcode, "D+A", 3))
        return COMP_D_PLUS_A;
    else if (STR_CMP(opcode, "D-A", 3))
        return COMP_D_MINUS_A;
    else if (STR_CMP(opcode, "A-D", 3))
        return COMP_A_MINUS_D;
    else if (STR_CMP(opcode, "D&A", 3))
        return COMP_D_AND_A;
    else if (STR_CMP(opcode, "D|A", 3))
        return COMP_D_OR_A;
    else if (STR_CMP(opcode, "M", 1)) {
        switch (type) {
            case hasm_comp:
                return COMP_M;
            case hasm_dest:
                return DEST_M;
            default:
                break;
        }

    } else if (STR_CMP(opcode, "!M", 2))
        return COMP_NOT_M;
    else if (STR_CMP(opcode, "-M", 2))
        return COMP_MINUS_M;
    else if (STR_CMP(opcode, "M+1", 3))
        return COMP_M_PLUS_1;
    else if (STR_CMP(opcode, "M-1", 3))
        return COMP_M_MINUS_1;
    else if (STR_CMP(opcode, "D+M", 3))
        return COMP_D_PLUS_M;
    else if (STR_CMP(opcode, "D-M", 3))
        return COMP_D_MINUS_M;
    else if (STR_CMP(opcode, "M-D", 3))
        return COMP_M_MINUS_D;
    else if (STR_CMP(opcode, "D&M", 3))
        return COMP_D_AND_M;
    else if (STR_CMP(opcode, "D|M", 3))
        return COMP_D_OR_M;
    else if (STR_CMP(opcode, "MD", 2))
        return DEST_MD;
    else if (STR_CMP(opcode, "AM", 2))
        return DEST_AM;
    else if (STR_CMP(opcode, "AD", 2))
        return DEST_AD;
    else if (STR_CMP(opcode, "AMD", 3))
        return DEST_AMD;
    else if (STR_CMP(opcode, "JGT", 3))
        return JGT;
    else if (STR_CMP(opcode, "JEQ", 3))
        return JEQ;
    else if (STR_CMP(opcode, "JGE", 3))
        return JGE;
    else if (STR_CMP(opcode, "JLT", 3))
        return JLT;
    else if (STR_CMP(opcode, "JNE", 3))
        return JNE;
    else if (STR_CMP(opcode, "JLE", 3))
        return JLE;
    else if (STR_CMP(opcode, "JMP", 3))
        return JMP;
return 0;
}
