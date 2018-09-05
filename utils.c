#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "utils.h"


void hasm_error(const char *fmt,int severity, ...) {
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    (severity == Fatal || severity == Error) ? exit(EXIT_FAILURE) : NULL;
}


void *hasm_malloc(size_t size) {
    void *p = malloc(size);
    if (!p) {
        hasm_error("unable to allocate memory", Fatal);
        exit(1);
    }
    return p;
}


FILE *hasm_fopen(const char *fname, const char *modes) {
    FILE *f;
    f = fopen(fname, modes);
    (!f) ? hasm_error("unable to open file", Error) : NULL;
    return f;
}

size_t hasm_fwrite(const void *ptr, size_t size, size_t count, FILE *fp) {
    size_t c = fwrite(ptr, size, count, fp);
    if (ferror(fp)) {
        hasm_error("error writing to file", Error);
        hasm_fclose(fp);
    }
    return c;
}

int hasm_fclose(FILE *fp) {
    if (fclose(fp) == EOF) {
        hasm_error("error closing file", Error);
        return EOF;
    }
    return 0;

}
