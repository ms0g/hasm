#include <string.h>
#include "optab.h"
#include "hopcodes.h"


u16 scan_opc(const char *opcode, const enum op_type type) {
    if (opcode == NULL) {
        return 0;
    }
    if (STR_CMP(opcode, "0"))
        return COMP_ZERO;
    else if (STR_CMP(opcode, "1"))
        return COMP_ONE;
    else if (STR_CMP(opcode, "-1"))
        return COMP_MINUS_1;
    else if (STR_CMP(opcode, "D")) {
        switch (type) {
            case hasm_comp:
                return COMP_D;
            case hasm_dest:
                return DEST_D;
            default:
                break;
        }
    } else if (STR_CMP(opcode, "A")) {
        switch (type) {
            case hasm_comp:
                return COMP_A;
            case hasm_dest:
                return DEST_A;
            default:
                break;
        }
    } else if (STR_CMP(opcode, "!D"))
        return COMP_NOT_D;
    else if (STR_CMP(opcode, "!A"))
        return COMP_NOT_A;
    else if (STR_CMP(opcode, "-D"))
        return COMP_MINUS_D;
    else if (STR_CMP(opcode, "-A"))
        return COMP_MINUS_A;
    else if (STR_CMP(opcode, "D+1"))
        return COMP_D_PLUS_1;
    else if (STR_CMP(opcode, "A+1"))
        return COMP_A_PLUS_1;
    else if (STR_CMP(opcode, "D-1"))
        return COMP_D_MINUS_1;
    else if (STR_CMP(opcode, "A-1"))
        return COMP_A_MINUS_1;
    else if (STR_CMP(opcode, "D+A"))
        return COMP_D_PLUS_A;
    else if (STR_CMP(opcode, "D-A"))
        return COMP_D_MINUS_A;
    else if (STR_CMP(opcode, "A-D"))
        return COMP_A_MINUS_D;
    else if (STR_CMP(opcode, "D&A"))
        return COMP_D_AND_A;
    else if (STR_CMP(opcode, "D|A"))
        return COMP_D_OR_A;
    else if (STR_CMP(opcode, "M")) {
        switch (type) {
            case hasm_comp:
                return COMP_M;
            case hasm_dest:
                return DEST_M;
            default:
                break;
        }

    } else if (STR_CMP(opcode, "!M"))
        return COMP_NOT_M;
    else if (STR_CMP(opcode, "-M"))
        return COMP_MINUS_M;
    else if (STR_CMP(opcode, "M+1"))
        return COMP_M_PLUS_1;
    else if (STR_CMP(opcode, "M-1"))
        return COMP_M_MINUS_1;
    else if (STR_CMP(opcode, "D+M"))
        return COMP_D_PLUS_M;
    else if (STR_CMP(opcode, "D-M"))
        return COMP_D_MINUS_M;
    else if (STR_CMP(opcode, "M-D"))
        return COMP_M_MINUS_D;
    else if (STR_CMP(opcode, "D&M"))
        return COMP_D_AND_M;
    else if (STR_CMP(opcode, "D|M"))
        return COMP_D_OR_M;
    else if (STR_CMP(opcode, "MD"))
        return DEST_MD;
    else if (STR_CMP(opcode, "AM"))
        return DEST_AM;
    else if (STR_CMP(opcode, "AD"))
        return DEST_AD;
    else if (STR_CMP(opcode, "AMD"))
        return DEST_AMD;
    else if (STR_CMP(opcode, "JGT"))
        return JGT;
    else if (STR_CMP(opcode, "JEQ"))
        return JEQ;
    else if (STR_CMP(opcode, "JGE"))
        return JGE;
    else if (STR_CMP(opcode, "JLT"))
        return JLT;
    else if (STR_CMP(opcode, "JNE"))
        return JNE;
    else if (STR_CMP(opcode, "JLE"))
        return JLE;
    else if (STR_CMP(opcode, "JMP"))
        return JMP;
return 0;
}
