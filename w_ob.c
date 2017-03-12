#include <string.h>
#include <stdio.h>
#include "w_ob.h"


void write_hdr(FILE *bin_file){
    Hof_hdr hdr;
    hdr.h_MAG0 = MAG0;
    hdr.h_MAG1 = MAG1;
    hdr.h_MAG2 = MAG2;
    hdr.h_MAG3 = MAG3;
    hdr.h_end = END;
    hdr.p_off = P_OFF;

    fwrite(&hdr,sizeof(Hof_hdr), 1, bin_file);
}
