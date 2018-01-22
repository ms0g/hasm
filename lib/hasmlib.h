#ifndef HASM_HASMLIB_H
#define HASM_HASMLIB_H
#include <stdio.h>

#define read_msb(_val_) (((_val_) << 8)|((_val_) >> 8)) /* read byte MSB */

void hasm_error(const char *fmt, ...);
void *hasm_malloc(size_t size);
int hasm_fclose(FILE *fp);
FILE* hasm_fopen(const char *fname, const char *modes);
size_t hasm_fwrite(const void *ptr, size_t size, size_t count, FILE *fp);

#endif //HASM_HASMLIB_H
