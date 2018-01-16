#ifndef HASM_HASMLIB_H
#define HASM_HASMLIB_H
#include <stdio.h>

#define read_msb(_val_) (((_val_) << 8)|((_val_) >> 8)) /* read byte MSB */

void *hmalloc(size_t size);
int hfclose(FILE *fp);
FILE* hfopen(const char *fname, const char *modes);
size_t hfwrite(const void *ptr, size_t size, size_t count, FILE *fp);

#endif //HASM_HASMLIB_H
