#include <stdint.h>
#include <string.h>
#include "optab.h"
#include "utils.h"
#include "hopcodes.h"

uint16_t scan_opc(const char *opcode, const enum op_type type) {
    if (strcmp(opcode, "0") == 0)
        return COMP_ZERO;
    else if (strcmp(opcode, "1") == 0)
        return COMP_ONE;
    else if (strcmp(opcode, "-1") == 0)
        return COMP_MINUS_1;
    else if (strcmp(opcode, "D") == 0) {
        switch (type) {
            case hasm_comp:
                return COMP_D;
            case hasm_dest:
                return DEST_D;
            default:
                break;
        }
    } else if (strcmp(opcode, "A") == 0) {
        switch (type) {
            case hasm_comp:
                return COMP_A;
            case hasm_dest:
                return DEST_A;
            default:
                break;
        }
    } else if (strcmp(opcode, "!D") == 0)
        return COMP_NOT_D;
    else if (strcmp(opcode, "!A") == 0)
        return COMP_NOT_A;
    else if (strcmp(opcode, "-D") == 0)
        return COMP_MINUS_D;
    else if (strcmp(opcode, "-A") == 0)
        return COMP_MINUS_A;
    else if (strcmp(opcode, "D+1") == 0)
        return COMP_D_PLUS_1;
    else if (strcmp(opcode, "A+1") == 0)
        return COMP_A_PLUS_1;
    else if (strcmp(opcode, "D-1") == 0)
        return COMP_D_MINUS_1;
    else if (strcmp(opcode, "A-1") == 0)
        return COMP_A_MINUS_1;
    else if (strcmp(opcode, "D+A") == 0)
        return COMP_D_PLUS_A;
    else if (strcmp(opcode, "D-A") == 0)
        return COMP_D_MINUS_A;
    else if (strcmp(opcode, "A-D") == 0)
        return COMP_A_MINUS_D;
    else if (strcmp(opcode, "D&A") == 0)
        return COMP_D_AND_A;
    else if (strcmp(opcode, "D|A") == 0)
        return COMP_D_OR_A;
    else if (strcmp(opcode, "M") == 0) {
        switch (type) {
            case hasm_comp:
                return COMP_M;
            case hasm_dest:
                return DEST_M;
            default:
                break;
        }

    } else if (strcmp(opcode, "!M") == 0)
        return COMP_NOT_M;
    else if (strcmp(opcode, "-M") == 0)
        return COMP_MINUS_M;
    else if (strcmp(opcode, "M+1") == 0)
        return COMP_M_PLUS_1;
    else if (strcmp(opcode, "M-1") == 0)
        return COMP_M_MINUS_1;
    else if (strcmp(opcode, "D+M") == 0)
        return COMP_D_PLUS_M;
    else if (strcmp(opcode, "D-M") == 0)
        return COMP_D_MINUS_M;
    else if (strcmp(opcode, "M-D") == 0)
        return COMP_M_MINUS_D;
    else if (strcmp(opcode, "D&M") == 0)
        return COMP_D_AND_M;
    else if (strcmp(opcode, "D|M") == 0)
        return COMP_D_OR_M;
    else if (strcmp(opcode, "MD") == 0)
        return DEST_MD;
    else if (strcmp(opcode, "AM") == 0)
        return DEST_AM;
    else if (strcmp(opcode, "AD") == 0)
        return DEST_AD;
    else if (strcmp(opcode, "AMD") == 0)
        return DEST_AMD;
    else if (strcmp(opcode, "JGT") == 0)
        return JGT;
    else if (strcmp(opcode, "JEQ") == 0)
        return JEQ;
    else if (strcmp(opcode, "JGE") == 0)
        return JGE;
    else if (strcmp(opcode, "JLT") == 0)
        return JLT;
    else if (strcmp(opcode, "JNE") == 0)
        return JNE;
    else if (strcmp(opcode, "JLE") == 0)
        return JLE;
    else if (strcmp(opcode, "JMP") == 0)
        return JMP;
    else
        return 0;
}