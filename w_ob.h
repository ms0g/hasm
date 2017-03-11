#ifndef HASM_W_OB_H
#define HASM_W_OB_H

#define unint16_read(_val_) (_val_ << 8)|(_val_ >> 8)


typedef struct {
    unsigned char h_MAG0;
    unsigned char h_MAG1;
    unsigned char h_MAG2;
    unsigned char h_MAG3;
    unsigned short h_end;
    unsigned short p_off;

}Hof_hdr;

#define MAG0 0x48
#define MAG1 0x41
#define MAG2 0x43
#define MAG3 0x4B
#define END unint16_read(0x01)
#define P_OFF unint16_read(0x08)

void write_hdr(FILE *bin_file);
#endif //HASM_W_OB_H
