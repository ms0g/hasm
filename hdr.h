#ifndef HASM_HDR_H
#define HASM_HDR_H

#include <stdint.h>
#include "utils.h"

/* hack exec file header */
typedef struct {
    u8 h_MAG0;
    u8 h_MAG1;
    u8 h_MAG2;
    u8 h_MAG3;
    unsigned short h_end;
    unsigned short p_off;

} Hex_hdr;

#define MAG0 0x48 /* 'H' */
#define MAG1 0x41 /* 'A' */
#define MAG2 0x43 /* 'C' */
#define MAG3 0x4B /* 'K' */
#define END read_msb(0x01) /* Endianness MSB(1) or LSB(2) */
#define P_OFF read_msb(0x08) /* Program offset */

/* Write header */
void write_hdr(FILE *fp);

#endif //HASM_HDR_H
