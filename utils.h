#ifndef HASM_UTILS_H
#define HASM_UTILS_H
#include <stdio.h>
#include <stdint.h>


typedef uint8_t u8;
typedef uint16_t u16;

#define read_msb(_val_) (((_val_) << 8)|((_val_) >> 8)) /* read byte MSB */
#define STR_CMP(s1, s2, n) (strcmp(s1, s2) == 0)

#define Hasm_Free(ptr) {    \
    if (ptr != NULL) {      \
        free(ptr);          \
        ptr=NULL;           \
    }                       \
}

enum error_severity {
    Fatal,
    Error,
    Warning,
    Info
};

void hasm_error(const char *fmt,int severity, ...);
void *hasm_malloc(size_t size);
int hasm_fclose(FILE *fp);
FILE* hasm_fopen(const char *filename, const char *modes);
size_t hasm_fwrite(const void *ptr, size_t size, size_t count, FILE *fp);
int fd_isreg(const char *filename);

#endif //HASM_UTILS_H
