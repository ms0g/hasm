#include <string.h>
#include <stdio.h>
#include "hdr.h"


void write_hdr(FILE *ofile){
    Hof_hdr hdr;
    hdr.h_MAG0 = MAG0;
    hdr.h_MAG1 = MAG1;
    hdr.h_MAG2 = MAG2;
    hdr.h_MAG3 = MAG3;
    hdr.h_end = END;
    hdr.p_off = P_OFF;

    hfwrite(&hdr,sizeof(Hof_hdr), 1, ofile);
}
