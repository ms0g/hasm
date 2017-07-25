#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "hasmlib.h"

void *hmalloc(size_t size){
    void *p = malloc(size);
    if(!p){
        fprintf(stderr, "unable to allocate memory");
        exit(1);
    }
    return p;
}


FILE* hfopen(const char *fname, const char *modes){
    FILE *f;
    f = fopen(fname, modes);
    if (!f){
        fprintf(stderr, "unable to open file");
        exit(1);
    }
    return f;
}

size_t hfwrite(const void *ptr, size_t size, size_t count, FILE *stream){
    size_t c = fwrite(ptr, size, count, stream);
    if (ferror(stream)) {
        fprintf(stderr, "error writing to file");
        hfclose(stream);
    }
    return c;
}

int hfclose(FILE *stream){
    if(fclose(stream) == EOF){
        fprintf(stderr, "error closing file");
        return EOF;
    }
    return 0;

}
