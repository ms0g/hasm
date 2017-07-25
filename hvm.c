/*
 * hvm.c
 */

#include <getopt.h>
#include <memory.h>
#include <stdlib.h>
#include <assert.h>
#include "hvm.h"

#define TRUE    1
#define FALSE   0
#define INVALID -1
#define P_OFF 0x8 /* program offset */

/* Current state of machine */
int running = TRUE;

/* VM initialization */
void vm_init(char *arg) {
    uint16_t buff;
    int ind = 0;

    FILE *hex;
    hex = fopen(arg, "rb");

    assert(hex != NULL);

    /* jump program offset */
    fseek(hex, P_OFF, SEEK_SET);
    while (fread(&buff, sizeof(buff), 1, hex) != 0) {
        ROM[ind++] = read_msb(buff);
    }
    /* our end-of-program signature */
    ROM[ind] = (uint16_t) EOF;
    fclose(hex);
}

/* Fetch */
uint16_t fetch(HVMData *hdt) {
    return ROM[hdt->pc++];
}

/* Decode */
void decode(uint16_t instr, HVMData *hdt) {
    /* check instr first 3 bits.If 111 it is C instr,otherwise A instr */
    if (((instr & 0xE000) >> 13) ^ 0x7) {
        hdt->A_REG = instr;
        return;
    }
    /* extract comp,dest,jmp parts of instr via bit masking */
    hdt->comp = (uint16_t) ((instr & 0xFFC0) >> 6); // 1111111111000000
    hdt->dest = (uint8_t) ((instr & 0x38) >> 3); //  0000000000111000
    hdt->jmp = (uint8_t) (instr & 0x07); // 0000000000000111

    /* turn machine state execute */
    hdt->ex_state = TRUE;
}

/* Execute */
void execute(HVMData *hdt) {
    if (!(hdt->jmp ^ 0x0)) {
        switch (hdt->comp) {
            case COMP_ZERO:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = 0x0;
                        break;
                    case DEST_D:
                        hdt->D_REG = 0x0;
                        break;
                    case DEST_MD:
                        hdt->D_REG = 0x0;
                        RAM[hdt->A_REG] = 0x0;
                        break;
                    case DEST_A:
                        hdt->A_REG = 0x0;
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = 0x0;
                        hdt->A_REG = 0x0;
                        break;
                    case DEST_AD:
                        hdt->D_REG = 0x0;
                        hdt->A_REG = 0x0;
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = 0x0;
                        hdt->A_REG = 0x0;
                        hdt->D_REG = 0x0;
                        break;
                    default:
                        break;
                }

                break;
            case COMP_ONE:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = 0x1;
                        break;
                    case DEST_D:
                        hdt->D_REG = 0x1;
                        break;
                    case DEST_MD:
                        hdt->D_REG = 0x1;
                        RAM[hdt->A_REG] = 0x1;
                        break;
                    case DEST_A:
                        hdt->A_REG = 0x1;
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = 0x1;
                        hdt->A_REG = 0x1;
                        break;
                    case DEST_AD:
                        hdt->D_REG = 0x1;
                        hdt->A_REG = 0x1;
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = 0x1;
                        hdt->A_REG = 0x1;
                        hdt->D_REG = 0x1;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_MINUS_1:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = -1;
                        break;
                    case DEST_D:
                        hdt->D_REG = -1;
                        break;
                    case DEST_MD:
                        hdt->D_REG = -1;
                        RAM[hdt->A_REG] = -1;
                        break;
                    case DEST_A:
                        hdt->A_REG = -1;
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = -1;
                        hdt->A_REG = -1;
                        break;
                    case DEST_AD:
                        hdt->D_REG = -1;
                        hdt->A_REG = -1;
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = -1;
                        hdt->A_REG = -1;
                        hdt->D_REG = -1;
                        break;
                    default:
                        break;
                }

                break;
            case COMP_D:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = hdt->D_REG;
                        break;
                    case DEST_D:
                        break;
                    case DEST_MD:
                        RAM[hdt->A_REG] = hdt->D_REG;
                        break;
                    case DEST_A:
                        hdt->A_REG = hdt->D_REG;
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = hdt->D_REG;
                        hdt->A_REG = hdt->D_REG;
                        break;
                    case DEST_AD:
                        hdt->A_REG = hdt->D_REG;
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = hdt->D_REG;
                        hdt->A_REG = hdt->D_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_A:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = hdt->A_REG;
                        break;
                    case DEST_D:
                        hdt->D_REG = hdt->A_REG;
                        break;
                    case DEST_MD:
                        hdt->D_REG = hdt->A_REG;
                        RAM[hdt->A_REG] = hdt->A_REG;
                        break;
                    case DEST_A:
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = hdt->A_REG;
                        break;
                    case DEST_AD:
                        hdt->D_REG = hdt->A_REG;
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = hdt->A_REG;
                        hdt->D_REG = hdt->A_REG;
                        break;
                    default:
                        break;
                }

                break;
            case COMP_NOT_D:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = ~(hdt->D_REG);
                        break;
                    case DEST_D:
                        hdt->D_REG = ~(hdt->D_REG);
                        break;
                    case DEST_MD:
                        hdt->D_REG = ~(hdt->D_REG);
                        RAM[hdt->A_REG] = ~(hdt->D_REG);
                        break;
                    case DEST_A:
                        hdt->A_REG = ~(hdt->D_REG);
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = ~(hdt->D_REG);
                        hdt->A_REG = ~(hdt->D_REG);
                        break;
                    case DEST_AD:
                        hdt->D_REG = ~(hdt->D_REG);
                        hdt->A_REG = ~(hdt->D_REG);
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = ~(hdt->D_REG);
                        hdt->A_REG = ~(hdt->D_REG);
                        hdt->D_REG = ~(hdt->D_REG);
                        break;
                    default:
                        break;
                }
                break;
            case COMP_NOT_A:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = ~(hdt->A_REG);
                        break;
                    case DEST_D:
                        hdt->D_REG = ~(hdt->A_REG);
                        break;
                    case DEST_MD:
                        hdt->D_REG = ~(hdt->A_REG);
                        RAM[hdt->A_REG] = ~(hdt->A_REG);
                        break;
                    case DEST_A:
                        hdt->A_REG = ~(hdt->A_REG);
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = ~(hdt->A_REG);
                        hdt->A_REG = ~(hdt->A_REG);
                        break;
                    case DEST_AD:
                        hdt->D_REG = ~(hdt->A_REG);
                        hdt->A_REG = ~(hdt->A_REG);
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = ~(hdt->A_REG);
                        hdt->A_REG = ~(hdt->A_REG);
                        hdt->D_REG = ~(hdt->A_REG);
                        break;
                    default:
                        break;
                }
                break;
            case COMP_MINUS_D:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = -hdt->D_REG;
                        break;
                    case DEST_D:
                        hdt->D_REG = -hdt->D_REG;
                        break;
                    case DEST_MD:
                        hdt->D_REG = -hdt->D_REG;
                        RAM[hdt->A_REG] = -hdt->D_REG;
                        break;
                    case DEST_A:
                        hdt->A_REG = -hdt->D_REG;
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = -hdt->D_REG;
                        hdt->A_REG = -hdt->D_REG;
                        break;
                    case DEST_AD:
                        hdt->D_REG = -hdt->D_REG;
                        hdt->A_REG = -hdt->D_REG;
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = -hdt->D_REG;
                        hdt->A_REG = -hdt->D_REG;
                        hdt->D_REG = -hdt->D_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_MINUS_A:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = -hdt->A_REG;
                        break;
                    case DEST_D:
                        hdt->D_REG = -hdt->A_REG;
                        break;
                    case DEST_MD:
                        hdt->D_REG = -hdt->A_REG;
                        RAM[hdt->A_REG] = -hdt->A_REG;
                        break;
                    case DEST_A:
                        hdt->A_REG = -hdt->A_REG;
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = -hdt->A_REG;
                        hdt->A_REG = -hdt->A_REG;
                        break;
                    case DEST_AD:
                        hdt->D_REG = -hdt->A_REG;
                        hdt->A_REG = -hdt->A_REG;
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = -hdt->A_REG;
                        hdt->A_REG = -hdt->A_REG;
                        hdt->D_REG = -hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_PLUS_1:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = ++hdt->D_REG;
                        break;
                    case DEST_D:
                        ++hdt->D_REG;
                        break;
                    case DEST_MD:
                        ++hdt->D_REG;
                        RAM[hdt->A_REG] = hdt->D_REG;
                        break;
                    case DEST_A:
                        hdt->A_REG = ++hdt->D_REG;
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = ++hdt->D_REG;
                        hdt->A_REG = hdt->D_REG;
                        break;
                    case DEST_AD:
                        ++hdt->D_REG;
                        hdt->A_REG = hdt->D_REG;
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = ++hdt->D_REG;
                        hdt->A_REG = hdt->D_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_A_PLUS_1:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = ++hdt->A_REG;
                        break;
                    case DEST_D:
                        hdt->D_REG = ++hdt->A_REG;
                        break;
                    case DEST_MD:
                        hdt->D_REG = ++hdt->A_REG;
                        RAM[hdt->A_REG] = hdt->A_REG;
                        break;
                    case DEST_A:
                        ++hdt->A_REG;
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = ++hdt->A_REG;
                        break;
                    case DEST_AD:
                        hdt->D_REG = ++hdt->A_REG;
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = ++hdt->A_REG;
                        hdt->D_REG = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_MINUS_1:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = --hdt->D_REG;
                        break;
                    case DEST_D:
                        --hdt->D_REG;
                        break;
                    case DEST_MD:
                        --hdt->D_REG;
                        RAM[hdt->A_REG] = hdt->D_REG;
                        break;
                    case DEST_A:
                        hdt->A_REG = --hdt->D_REG;
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = --hdt->D_REG;
                        hdt->A_REG = hdt->D_REG;
                        break;
                    case DEST_AD:
                        --hdt->D_REG;
                        hdt->A_REG = hdt->D_REG;
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = --hdt->D_REG;
                        hdt->A_REG = hdt->D_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_A_MINUS_1:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = --hdt->A_REG;
                        break;
                    case DEST_D:
                        hdt->D_REG = --hdt->A_REG;
                        break;
                    case DEST_MD:
                        hdt->D_REG = --hdt->A_REG;
                        RAM[hdt->A_REG] = hdt->A_REG;
                        break;
                    case DEST_A:
                        --hdt->A_REG;
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = --hdt->A_REG;
                        break;
                    case DEST_AD:
                        hdt->D_REG = --hdt->A_REG;
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = --hdt->A_REG;
                        hdt->D_REG = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_PLUS_A:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = hdt->D_REG + hdt->A_REG;
                        break;
                    case DEST_D:
                        hdt->D_REG += hdt->A_REG;
                        break;
                    case DEST_MD:
                        hdt->D_REG += hdt->A_REG;
                        RAM[hdt->A_REG] = hdt->D_REG + hdt->A_REG;
                        break;
                    case DEST_A:
                        hdt->A_REG += hdt->D_REG;
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = hdt->D_REG + hdt->A_REG;
                        hdt->A_REG += hdt->D_REG;
                        break;
                    case DEST_AD:
                        hdt->D_REG += hdt->A_REG;
                        hdt->A_REG += hdt->D_REG;
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = hdt->D_REG + hdt->A_REG;
                        hdt->A_REG += hdt->D_REG;
                        hdt->D_REG += hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_MINUS_A:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = hdt->D_REG - hdt->A_REG;
                        break;
                    case DEST_D:
                        hdt->D_REG -= hdt->A_REG;
                        break;
                    case DEST_MD:
                        hdt->D_REG -= hdt->A_REG;
                        RAM[hdt->A_REG] = hdt->D_REG - hdt->A_REG;
                        break;
                    case DEST_A:
                        hdt->A_REG = hdt->D_REG - hdt->A_REG;
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = hdt->D_REG - hdt->A_REG;
                        hdt->A_REG = hdt->D_REG - hdt->A_REG;
                        break;
                    case DEST_AD:
                        hdt->D_REG -= hdt->A_REG;
                        hdt->A_REG = hdt->D_REG - hdt->A_REG;
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = hdt->D_REG - hdt->A_REG;
                        hdt->A_REG = hdt->D_REG - hdt->A_REG;
                        hdt->D_REG -= hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_A_MINUS_D:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = hdt->A_REG - hdt->D_REG;
                        break;
                    case DEST_D:
                        hdt->D_REG = hdt->A_REG - hdt->D_REG;
                        break;
                    case DEST_MD:
                        hdt->D_REG = hdt->A_REG - hdt->D_REG;
                        RAM[hdt->A_REG] = hdt->A_REG - hdt->D_REG;
                        break;
                    case DEST_A:
                        hdt->A_REG -= hdt->D_REG;
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = hdt->A_REG - hdt->D_REG;
                        hdt->A_REG -= hdt->D_REG;
                        break;
                    case DEST_AD:
                        hdt->D_REG = hdt->A_REG - hdt->D_REG;
                        hdt->A_REG -= hdt->D_REG;
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = hdt->A_REG - hdt->D_REG;
                        hdt->A_REG -= hdt->D_REG;
                        hdt->D_REG = hdt->A_REG - hdt->D_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_AND_A:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = hdt->D_REG & hdt->A_REG;
                        break;
                    case DEST_D:
                        hdt->D_REG &= hdt->A_REG;
                        break;
                    case DEST_MD:
                        hdt->D_REG &= hdt->A_REG;
                        RAM[hdt->A_REG] = hdt->D_REG & hdt->A_REG;
                        break;
                    case DEST_A:
                        hdt->A_REG = hdt->D_REG & hdt->A_REG;
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = hdt->D_REG & hdt->A_REG;
                        hdt->A_REG = hdt->D_REG & hdt->A_REG;
                        break;
                    case DEST_AD:
                        hdt->D_REG &= hdt->A_REG;
                        hdt->A_REG = hdt->D_REG & hdt->A_REG;
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = hdt->D_REG & hdt->A_REG;
                        hdt->A_REG = hdt->D_REG & hdt->A_REG;
                        hdt->D_REG &= hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_OR_A:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = hdt->D_REG | hdt->A_REG;
                        break;
                    case DEST_D:
                        hdt->D_REG |= hdt->A_REG;
                        break;
                    case DEST_MD:
                        hdt->D_REG |= hdt->A_REG;
                        RAM[hdt->A_REG] = hdt->D_REG | hdt->A_REG;
                        break;
                    case DEST_A:
                        hdt->A_REG = hdt->D_REG | hdt->A_REG;
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = hdt->D_REG | hdt->A_REG;
                        hdt->A_REG = hdt->D_REG | hdt->A_REG;
                        break;
                    case DEST_AD:
                        hdt->D_REG |= hdt->A_REG;
                        hdt->A_REG = hdt->D_REG | hdt->A_REG;
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = hdt->D_REG | hdt->A_REG;
                        hdt->A_REG = hdt->D_REG | hdt->A_REG;
                        hdt->D_REG |= hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_M:
                switch (hdt->dest) {
                    case DEST_M:
                        break;
                    case DEST_D:
                        hdt->D_REG = RAM[hdt->A_REG];
                        break;
                    case DEST_MD:
                        hdt->D_REG = RAM[hdt->A_REG];
                        break;
                    case DEST_A:
                        hdt->A_REG = RAM[hdt->A_REG];
                        break;
                    case DEST_AM:
                        hdt->A_REG = RAM[hdt->A_REG];
                        break;
                    case DEST_AD:
                        hdt->D_REG = RAM[hdt->A_REG];
                        hdt->A_REG = RAM[hdt->A_REG];
                        break;
                    case DEST_AMD:
                        hdt->A_REG = RAM[hdt->A_REG];
                        hdt->D_REG = RAM[hdt->A_REG];
                        break;
                    default:
                        break;
                }
                break;
            case COMP_NOT_M:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = ~RAM[hdt->A_REG];
                        break;
                    case DEST_D:
                        hdt->D_REG = ~RAM[hdt->A_REG];
                        break;
                    case DEST_MD:
                        hdt->D_REG = ~RAM[hdt->A_REG];
                        RAM[hdt->A_REG] = ~RAM[hdt->A_REG];
                        break;
                    case DEST_A:
                        hdt->A_REG = ~RAM[hdt->A_REG];
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = ~RAM[hdt->A_REG];
                        hdt->A_REG = ~RAM[hdt->A_REG];
                        break;
                    case DEST_AD:
                        hdt->D_REG = ~RAM[hdt->A_REG];
                        hdt->A_REG = ~RAM[hdt->A_REG];
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = ~RAM[hdt->A_REG];
                        hdt->A_REG = ~RAM[hdt->A_REG];
                        hdt->D_REG = ~RAM[hdt->A_REG];
                        break;
                    default:
                        break;
                }
                break;
            case COMP_MINUS_M:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = -RAM[hdt->A_REG];
                        break;
                    case DEST_D:
                        hdt->D_REG = -RAM[hdt->A_REG];
                        break;
                    case DEST_MD:
                        hdt->D_REG = -RAM[hdt->A_REG];
                        RAM[hdt->A_REG] = -RAM[hdt->A_REG];
                        break;
                    case DEST_A:
                        hdt->A_REG = -RAM[hdt->A_REG];
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = -RAM[hdt->A_REG];
                        hdt->A_REG = -RAM[hdt->A_REG];
                        break;
                    case DEST_AD:
                        hdt->D_REG = -RAM[hdt->A_REG];
                        hdt->A_REG = -RAM[hdt->A_REG];
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = -RAM[hdt->A_REG];
                        hdt->A_REG = -RAM[hdt->A_REG];
                        hdt->D_REG = -RAM[hdt->A_REG];
                        break;
                    default:
                        break;
                }
                break;
            case COMP_M_PLUS_1:
                switch (hdt->dest) {
                    case DEST_M:
                        ++RAM[hdt->A_REG];
                        break;
                    case DEST_D:
                        hdt->D_REG = ++RAM[hdt->A_REG];
                        break;
                    case DEST_MD:
                        hdt->D_REG = ++RAM[hdt->A_REG];
                        break;
                    case DEST_A:
                        hdt->A_REG = ++RAM[hdt->A_REG];
                        break;
                    case DEST_AM:
                        ++RAM[hdt->A_REG];
                        hdt->A_REG = RAM[hdt->A_REG];
                        break;
                    case DEST_AD:
                        hdt->D_REG = ++RAM[hdt->A_REG];
                        hdt->A_REG = RAM[hdt->A_REG];
                        break;
                    case DEST_AMD:
                        ++RAM[hdt->A_REG];
                        hdt->A_REG = RAM[hdt->A_REG];
                        hdt->D_REG = RAM[hdt->A_REG];
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_PLUS_M:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = hdt->D_REG + RAM[hdt->A_REG];
                        break;
                    case DEST_D:
                        hdt->D_REG += RAM[hdt->A_REG];
                        break;
                    case DEST_MD:
                        hdt->D_REG += RAM[hdt->A_REG];
                        RAM[hdt->A_REG] = hdt->D_REG + RAM[hdt->A_REG];
                        break;
                    case DEST_A:
                        hdt->A_REG = hdt->D_REG + RAM[hdt->A_REG];
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = hdt->D_REG + RAM[hdt->A_REG];
                        hdt->A_REG = hdt->D_REG + RAM[hdt->A_REG];
                        break;
                    case DEST_AD:
                        hdt->D_REG += RAM[hdt->A_REG];
                        hdt->A_REG = hdt->D_REG + RAM[hdt->A_REG];
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = hdt->D_REG + RAM[hdt->A_REG];
                        hdt->A_REG = hdt->D_REG + RAM[hdt->A_REG];
                        hdt->D_REG += RAM[hdt->A_REG];
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_MINUS_M:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = hdt->D_REG - RAM[hdt->A_REG];
                        break;
                    case DEST_D:
                        hdt->D_REG -= RAM[hdt->A_REG];
                        break;
                    case DEST_MD:
                        hdt->D_REG -= RAM[hdt->A_REG];
                        RAM[hdt->A_REG] = hdt->D_REG - RAM[hdt->A_REG];
                        break;
                    case DEST_A:
                        hdt->A_REG = hdt->D_REG - RAM[hdt->A_REG];
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = hdt->D_REG - RAM[hdt->A_REG];
                        hdt->A_REG = hdt->D_REG - RAM[hdt->A_REG];
                        break;
                    case DEST_AD:
                        hdt->D_REG -= RAM[hdt->A_REG];
                        hdt->A_REG = hdt->D_REG - RAM[hdt->A_REG];
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = hdt->D_REG - RAM[hdt->A_REG];
                        hdt->A_REG = hdt->D_REG - RAM[hdt->A_REG];
                        hdt->D_REG -= RAM[hdt->A_REG];
                        break;
                    default:
                        break;
                }
                break;
            case COMP_M_MINUS_D:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] -= hdt->D_REG;
                        break;
                    case DEST_D:
                        hdt->D_REG = RAM[hdt->A_REG] - hdt->D_REG;
                        break;
                    case DEST_MD:
                        hdt->D_REG = RAM[hdt->A_REG] - hdt->D_REG;
                        RAM[hdt->A_REG] -= hdt->D_REG;
                        break;
                    case DEST_A:
                        hdt->A_REG = RAM[hdt->A_REG] - hdt->D_REG;
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] -= hdt->D_REG;
                        hdt->A_REG = RAM[hdt->A_REG] - hdt->D_REG;
                        break;
                    case DEST_AD:
                        hdt->D_REG = RAM[hdt->A_REG] - hdt->D_REG;
                        hdt->A_REG = RAM[hdt->A_REG] - hdt->D_REG;
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] -= hdt->D_REG;
                        hdt->A_REG = RAM[hdt->A_REG] - hdt->D_REG;
                        hdt->D_REG = RAM[hdt->A_REG] - hdt->D_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_AND_M:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = hdt->D_REG & RAM[hdt->A_REG];
                        break;
                    case DEST_D:
                        hdt->D_REG &= RAM[hdt->A_REG];
                        break;
                    case DEST_MD:
                        hdt->D_REG &= RAM[hdt->A_REG];
                        RAM[hdt->A_REG] = hdt->D_REG & RAM[hdt->A_REG];
                        break;
                    case DEST_A:
                        hdt->A_REG = hdt->D_REG & RAM[hdt->A_REG];
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = hdt->D_REG & RAM[hdt->A_REG];
                        hdt->A_REG = hdt->D_REG & RAM[hdt->A_REG];
                        break;
                    case DEST_AD:
                        hdt->D_REG &= RAM[hdt->A_REG];
                        hdt->A_REG = hdt->D_REG & RAM[hdt->A_REG];
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = hdt->D_REG & RAM[hdt->A_REG];
                        hdt->A_REG = hdt->D_REG & RAM[hdt->A_REG];
                        hdt->D_REG &= RAM[hdt->A_REG];
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_OR_M:
                switch (hdt->dest) {
                    case DEST_M:
                        RAM[hdt->A_REG] = hdt->D_REG | RAM[hdt->A_REG];
                        break;
                    case DEST_D:
                        hdt->D_REG |= RAM[hdt->A_REG];
                        break;
                    case DEST_MD:
                        hdt->D_REG |= RAM[hdt->A_REG];
                        RAM[hdt->A_REG] = hdt->D_REG | RAM[hdt->A_REG];
                        break;
                    case DEST_A:
                        hdt->A_REG = hdt->D_REG | RAM[hdt->A_REG];
                        break;
                    case DEST_AM:
                        RAM[hdt->A_REG] = hdt->D_REG | RAM[hdt->A_REG];
                        hdt->A_REG = hdt->D_REG | RAM[hdt->A_REG];
                        break;
                    case DEST_AD:
                        hdt->D_REG |= RAM[hdt->A_REG];
                        hdt->A_REG = hdt->D_REG | RAM[hdt->A_REG];
                        break;
                    case DEST_AMD:
                        RAM[hdt->A_REG] = hdt->D_REG | RAM[hdt->A_REG];
                        hdt->A_REG = hdt->D_REG | RAM[hdt->A_REG];
                        hdt->D_REG |= RAM[hdt->A_REG];
                        break;
                    default:
                        break;
                }
                break;
            default:
                running = FALSE;
                break;
        }
    }
    else {
        switch (hdt->comp) {
            case COMP_ZERO:
                switch (hdt->jmp) {
                    case JGT:
                    case JLT:
                    case JNE:
                        break;
                    case JEQ:
                    case JGE:
                    case JLE:
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_ONE:
                switch (hdt->jmp) {
                    case JEQ:
                    case JLT:
                        break;
                    case JGT:
                    case JGE:
                    case JNE:
                    case JLE:
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_MINUS_1:
                switch (hdt->jmp) {
                    case JGT:
                    case JEQ:
                    case JGE:
                        break;
                    case JLT:
                    case JNE:
                    case JLE:
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }

                break;
            case COMP_D:
                switch (hdt->jmp) {
                    case JGT:
                        if (hdt->D_REG > 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JEQ:
                        if (hdt->D_REG == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if (hdt->D_REG >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if (hdt->D_REG < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if (hdt->D_REG != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if (hdt->D_REG <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_A:
                switch (hdt->jmp) {
                    case JGT:
                        if (hdt->A_REG > 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JEQ:
                        if (hdt->A_REG == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if (hdt->A_REG >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if (hdt->A_REG < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if (hdt->A_REG != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if (hdt->A_REG <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }

                break;
            case COMP_NOT_D:
                switch (hdt->jmp) {
                    case JGT:
                        if (~hdt->D_REG > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if (~hdt->D_REG == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if (~hdt->D_REG >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if (~hdt->D_REG < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if (~hdt->D_REG != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if (~hdt->D_REG <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_NOT_A:
                switch (hdt->jmp) {
                    case JGT:
                        if (~hdt->A_REG > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if (~hdt->A_REG == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if (~hdt->A_REG >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if (~hdt->A_REG < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if (~hdt->A_REG != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if (~hdt->A_REG <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_MINUS_D:
                switch (hdt->jmp) {
                    case JGT:
                        if (-hdt->D_REG > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if (-hdt->D_REG == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if (-hdt->D_REG >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if (-hdt->D_REG < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if (-hdt->D_REG != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if (-hdt->D_REG <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_MINUS_A:
                switch (hdt->jmp) {
                    case JGT:
                        if (-hdt->A_REG > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if (-hdt->A_REG == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if (-hdt->A_REG >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if (-hdt->A_REG < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if (-hdt->A_REG != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if (-hdt->A_REG <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_PLUS_1:
                switch (hdt->jmp) {
                    case JGT:
                        if (++hdt->D_REG > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if (++hdt->D_REG == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if (++hdt->D_REG >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if (++hdt->D_REG < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if (++hdt->D_REG != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if (++hdt->D_REG <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_A_PLUS_1:
                switch (hdt->jmp) {
                    case JGT:
                        if (++hdt->A_REG > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if (++hdt->A_REG == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if (++hdt->A_REG >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if (++hdt->A_REG < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if (++hdt->A_REG != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if (++hdt->A_REG <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_MINUS_1:
                switch (hdt->jmp) {
                    case JGT:
                        if (--hdt->D_REG > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if (--hdt->D_REG == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if (--hdt->D_REG >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if (--hdt->D_REG < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if (--hdt->D_REG != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if (--hdt->D_REG <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_A_MINUS_1:
                switch (hdt->jmp) {
                    case JGT:
                        if (--hdt->A_REG > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if (--hdt->A_REG == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if (--hdt->A_REG >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if (--hdt->A_REG < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if (--hdt->A_REG != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if (--hdt->A_REG <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_PLUS_A:
                switch (hdt->jmp) {
                    case JGT:
                        if ((hdt->D_REG + hdt->A_REG) > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if ((hdt->D_REG + hdt->A_REG) == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if ((hdt->D_REG + hdt->A_REG) >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if ((hdt->D_REG + hdt->A_REG) < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if ((hdt->D_REG + hdt->A_REG) != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if ((hdt->D_REG + hdt->A_REG) <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_MINUS_A:
                switch (hdt->jmp) {
                    case JGT:
                        if ((hdt->D_REG - hdt->A_REG) > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if ((hdt->D_REG - hdt->A_REG) == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if ((hdt->D_REG - hdt->A_REG) >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if ((hdt->D_REG - hdt->A_REG) < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if ((hdt->D_REG - hdt->A_REG) != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if ((hdt->D_REG - hdt->A_REG) <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_A_MINUS_D:
                switch (hdt->jmp) {
                    case JGT:
                        if ((hdt->A_REG - hdt->D_REG) > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if ((hdt->A_REG - hdt->D_REG) == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if ((hdt->A_REG - hdt->D_REG) >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if ((hdt->A_REG - hdt->D_REG) < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if ((hdt->A_REG - hdt->D_REG) != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if ((hdt->A_REG - hdt->D_REG) <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_AND_A:
                switch (hdt->jmp) {
                    case JGT:
                        if ((hdt->D_REG & hdt->A_REG) > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if ((hdt->D_REG & hdt->A_REG) == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if ((hdt->D_REG & hdt->A_REG) >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if ((hdt->D_REG & hdt->A_REG) < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if ((hdt->D_REG & hdt->A_REG) != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if ((hdt->D_REG & hdt->A_REG) <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_OR_A:
                switch (hdt->jmp) {
                    case JGT:
                        if ((hdt->D_REG | hdt->A_REG) > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if ((hdt->D_REG | hdt->A_REG) == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if ((hdt->D_REG | hdt->A_REG) >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if ((hdt->D_REG | hdt->A_REG) < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if ((hdt->D_REG | hdt->A_REG) != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if ((hdt->D_REG | hdt->A_REG) <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_M:
                switch (hdt->jmp) {
                    case JGT:
                        if (RAM[hdt->A_REG] > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if (RAM[hdt->A_REG] == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if (RAM[hdt->A_REG] >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if (RAM[hdt->A_REG] < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if (RAM[hdt->A_REG] != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if (RAM[hdt->A_REG] <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_NOT_M:
                switch (hdt->jmp) {
                    case JGT:
                        if (~RAM[hdt->A_REG] > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if (~RAM[hdt->A_REG] == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if (~RAM[hdt->A_REG] >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if (~RAM[hdt->A_REG] < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if (~RAM[hdt->A_REG] != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if (~RAM[hdt->A_REG] <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_MINUS_M:
                switch (hdt->jmp) {
                    case JGT:
                        if (-RAM[hdt->A_REG] > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if (-RAM[hdt->A_REG] == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if (-RAM[hdt->A_REG] >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if (-RAM[hdt->A_REG] < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if (-RAM[hdt->A_REG] != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if (-RAM[hdt->A_REG] <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_M_PLUS_1:
                switch (hdt->jmp) {
                    case JGT:
                        if (++RAM[hdt->A_REG] > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if (++RAM[hdt->A_REG] == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if (++RAM[hdt->A_REG] >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if (++RAM[hdt->A_REG] < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if (++RAM[hdt->A_REG] != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if (++RAM[hdt->A_REG] <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_PLUS_M:
                switch (hdt->jmp) {
                    case JGT:
                        if ((hdt->D_REG + RAM[hdt->A_REG]) > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if ((hdt->D_REG + RAM[hdt->A_REG]) == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if ((hdt->D_REG + RAM[hdt->A_REG]) >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if ((hdt->D_REG + RAM[hdt->A_REG]) < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if ((hdt->D_REG + RAM[hdt->A_REG]) != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if ((hdt->D_REG + RAM[hdt->A_REG]) <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_MINUS_M:
                switch (hdt->jmp) {
                    case JGT:
                        if ((hdt->D_REG - RAM[hdt->A_REG]) > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if ((hdt->D_REG - RAM[hdt->A_REG]) == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if ((hdt->D_REG - RAM[hdt->A_REG]) >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if ((hdt->D_REG - RAM[hdt->A_REG]) < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if ((hdt->D_REG - RAM[hdt->A_REG]) != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if ((hdt->D_REG - RAM[hdt->A_REG]) <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_M_MINUS_D:
                switch (hdt->jmp) {
                    case JGT:
                        if ((RAM[hdt->A_REG] - hdt->D_REG) > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if ((RAM[hdt->A_REG] - hdt->D_REG) == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if ((RAM[hdt->A_REG] - hdt->D_REG) >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if ((RAM[hdt->A_REG] - hdt->D_REG) < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if ((RAM[hdt->A_REG] - hdt->D_REG) != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if ((RAM[hdt->A_REG] - hdt->D_REG) <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_AND_M:
                switch (hdt->jmp) {
                    case JGT:
                        if ((hdt->D_REG & RAM[hdt->A_REG]) > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if ((hdt->D_REG & RAM[hdt->A_REG]) == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if ((hdt->D_REG & RAM[hdt->A_REG]) >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if ((hdt->D_REG & RAM[hdt->A_REG]) < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if ((hdt->D_REG & RAM[hdt->A_REG]) != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if ((hdt->D_REG & RAM[hdt->A_REG]) <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;
                }
                break;
            case COMP_D_OR_M:
                switch (hdt->jmp) {
                    case JGT:
                        if ((hdt->D_REG | RAM[hdt->A_REG]) > 0)
                            hdt->pc = hdt->A_REG;

                        break;
                    case JEQ:
                        if ((hdt->D_REG | RAM[hdt->A_REG]) == 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JGE:
                        if ((hdt->D_REG | RAM[hdt->A_REG]) >= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLT:
                        if ((hdt->D_REG | RAM[hdt->A_REG]) < 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JNE:
                        if ((hdt->D_REG | RAM[hdt->A_REG]) != 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JLE:
                        if ((hdt->D_REG | RAM[hdt->A_REG]) <= 0)
                            hdt->pc = hdt->A_REG;
                        break;
                    case JMP:
                        hdt->pc = hdt->A_REG;
                        break;
                    default:
                        break;

                }
                break;
            default:
                running = FALSE;
                break;
        }

    }
}

void snapshot(HVMData *hdt) {
    char *msg = " _   ___      ____  __   \n"
            "| | | |\\ \\   / |  \\/  |  \n"
            "| |_| | \\ \\ / /| |\\/| |  \n"
            "|  _  |  \\ V / | |  | |  \n"
            "|_| |_|   \\_/  |_|  |_|  \n"
            "                          \n"
            "Memory Snapshot \n"
            "****************************************\n"
            "*           *            *    CPU      *\n"
            "*           *            ***************\n"
            "*   ROM     *   RAM      |  A REG [%d]  \n"
            "*           *            |--------------\n"
            "*           *            |  D REG [%d]  \n"
            "*           *            |--------------\n"
            "*           *            |  PC [%d]     \n";

    char *rom = "_________________________\n"
                "|  %x             %d     \n";
    printf(msg, hdt->A_REG, hdt->D_REG, hdt->pc);
    for (int i = 0; ROM[i]^0xffff; ++i) {
        printf(rom, ROM[i],RAM[i]);
    }

}

int main(int argc, char *argv[]) {
    int opt;

    const char *usage = "Usage: ./hvm [file.hex]";
    while ((opt = getopt(argc, argv, "h:")) != -1) {
        switch (opt) {
            case 'h':
                printf("%s\n", usage);
                break;
            default: /* '?' */
                fprintf(stderr, "Usage: %s [file.hex]\n",
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (argv[optind] == NULL || strlen(argv[optind]) == 0) {
        printf("%s\n", usage);
        return 0;
    }

    vm_init(argv[optind]);

    uint16_t instr;
    HVMData hdt = {
            .ex_state=FALSE,
            .pc=0};

    while (running) {
        instr = fetch(&hdt);

        if (instr == INVALID)
            break;

        decode(instr, &hdt);
        if (hdt.ex_state) {
            hdt.ex_state = FALSE;
            execute(&hdt);
        }
    }
    snapshot(&hdt);
}
