#ifndef HASM_HDR_H
#define HASM_HDR_H
#include <stdint.h>
#include "hasmlib.h"


typedef struct {
    uint8_t h_MAG0;
    uint8_t h_MAG1;
    uint8_t h_MAG2;
    uint8_t h_MAG3;
    unsigned short h_end;
    unsigned short p_off;

}Hof_hdr; /* hack object file header */

#define MAG0 0x48 /* 'H' */
#define MAG1 0x41 /* 'A' */
#define MAG2 0x43 /* 'C' */
#define MAG3 0x4B /* 'K' */
#define END read_msb(0x01) /* Endianness MSB(1) or LSB(2) */
#define P_OFF read_msb(0x08) /* Program offset */

void write_hdr(FILE *ofile); /* write header */
#endif //HASM_HDR_H
