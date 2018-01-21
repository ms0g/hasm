#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "hasmlib.h"


void logging(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}


void *hmalloc(size_t size) {
    void *p = malloc(size);
    if (!p) {
        logging("unable to allocate memory");
        exit(1);
    }
    return p;
}


FILE *hfopen(const char *fname, const char *modes) {
    FILE *f;
    f = fopen(fname, modes);
    if (!f) {
        logging("unable to open file");
        exit(1);
    }
    return f;
}

size_t hfwrite(const void *ptr, size_t size, size_t count, FILE *fp) {
    size_t c = fwrite(ptr, size, count, fp);
    if (ferror(fp)) {
        logging("error writing to file");
        hfclose(fp);
    }
    return c;
}

int hfclose(FILE *fp) {
    if (fclose(fp) == EOF) {
        logging("error closing file");
        return EOF;
    }
    return 0;

}
