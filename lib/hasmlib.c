#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "hasmlib.h"


void hasm_error(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}


void *hasm_malloc(size_t size) {
    void *p = malloc(size);
    if (!p) {
        hasm_error("unable to allocate memory");
        exit(1);
    }
    return p;
}


FILE *hasm_fopen(const char *fname, const char *modes) {
    FILE *f;
    f = fopen(fname, modes);
    if (!f) {
        hasm_error("unable to open file");
        exit(1);
    }
    return f;
}

size_t hasm_fwrite(const void *ptr, size_t size, size_t count, FILE *fp) {
    size_t c = fwrite(ptr, size, count, fp);
    if (ferror(fp)) {
        hasm_error("error writing to file");
        hasm_fclose(fp);
    }
    return c;
}

int hasm_fclose(FILE *fp) {
    if (fclose(fp) == EOF) {
        hasm_error("error closing file");
        return EOF;
    }
    return 0;

}
