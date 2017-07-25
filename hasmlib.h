
#ifndef HASM_HASMLIB_H
#define HASM_HASMLIB_H

#define read_msb(_val_) (_val_ << 8)|(_val_ >> 8) /* read byte MSB */

void *hmalloc(size_t size);
FILE* hfopen(const char *fname,const char *modes);
int hfclose(FILE *stream);
size_t hfwrite(const void *ptr, size_t size, size_t count, FILE *stream);

#endif //HASM_HASMLIB_H
