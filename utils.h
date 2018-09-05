#ifndef HASM_HASMLIB_H
#define HASM_HASMLIB_H
#include <stdio.h>

#define read_msb(_val_) (((_val_) << 8)|((_val_) >> 8)) /* read byte MSB */

enum error_severity {
    Fatal,
    Error,
    Warning,
    Info
};

void hasm_error(const char *,int, ...);
void *hasm_malloc(size_t);
int hasm_fclose(FILE *);
FILE* hasm_fopen(const char *, const char *);
size_t hasm_fwrite(const void *, size_t, size_t, FILE *);
int fd_isreg(const char *);

#endif //HASM_HASMLIB_H
