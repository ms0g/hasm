#ifndef HASM_W_OB_H
#define HASM_W_OB_H

#define unint16_read(_val_) (_val_ << 8)|(_val_ >> 8) /* read byte MSB */


typedef struct {
    unsigned char h_MAG0;
    unsigned char h_MAG1;
    unsigned char h_MAG2;
    unsigned char h_MAG3;
    unsigned short h_end;
    unsigned short p_off;

}Hof_hdr; /* hack object file header */

#define MAG0 0x48 /* 'H' */
#define MAG1 0x41 /* 'A' */
#define MAG2 0x43 /* 'C' */
#define MAG3 0x4B /* 'K' */
#define END unint16_read(0x01) /* Endianness MSB(1) or LSB(2) */
#define P_OFF unint16_read(0x08) /* Program offset */

void write_hdr(FILE *bin_file); /* write header */
#endif //HASM_W_OB_H
