#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "parser.h"
#include "hdr.h"
#include "optab.h"
#include "tokenizer.h"
#include "lib/hasmlib.h"

#define BUFF 1080


/* buffer to keep binary field */
static uint16_t byte_buff;

/* address counter for label operands */
static uint16_t LC;

/* RAM[16] */
static uint16_t RAM_ADDR = 0x0010;


/* pass 1 */
void init_analysis(FILE *srcfp, char *file_name, struct Symbol **sym_tbl) {
    char buff[BUFF], token[100];
    FILE *int_file;
    struct Symbol *sym;
    int tok_type;

    memset(token, 0, 100);

    // create intermediate file
    strcat(file_name, ".int");
    int_file = hfopen(file_name, "w");

    while (fgets(buff, sizeof(buff), srcfp) != NULL) {
        sscanf(buff, "%s", buff);

        // ignore comments,spaces
        if (*buff == '/' || is_space(buff))
            continue;

        init_tokenizing(buff, token, &tok_type, NULL, pass1);

        switch (tok_type) {
            case LABEL:
                insert_symtab(sym_tbl, token, LC);
                break;
            case A_INS:
                sym = scan_symtab(*sym_tbl, token);
                (!sym) ? insert_symtab(sym_tbl, token, RAM_ADDR++) : NULL;
                break;
            case C_INS:
            default:
                break;
        }

        // flush buffer
        memset(token, 0, 100);

        if (tok_type != LABEL) {
            fprintf(int_file, "%s\n", buff);
            // increase ROM address
            ++LC;
        }
    }
    hfclose(int_file);
}

/* pass 2 */
void init_synthesis(char *file_name, struct Symbol **sym_tbl) {
    char buff[BUFF], token[100];
    unsigned short dest, comp, jmp;
    struct Symbol *sym;
    int tok_type;

    // int file
    intfp = hfopen(file_name, "r");

    char *t = strchr(file_name, '.');
    *t = '\0';
    strcat(file_name, ".hex");
    // output file
    outfp = hfopen(file_name, "wb");

    write_hdr(outfp);

    while (fgets(buff, sizeof(buff), intfp) != NULL) {
        C c_inst = {.comp="", .dest="", .jmp=""};

        sscanf(buff, "%s", buff);

        init_tokenizing(buff, token, &tok_type, &c_inst, pass2);

        switch (tok_type) {
            case A_INS:
                sym = scan_symtab(*sym_tbl, token);
                byte_buff |= (sym) ? sym->addr : 0;
                break;
            case C_INS:
                dest = scan_opc(c_inst.dest, hasm_dest);
                comp = scan_opc(c_inst.comp, hasm_comp);
                jmp = scan_opc(c_inst.jmp, hasm_jmp);

                byte_buff |= (dest && comp) ? (dest << 3 | comp << 6) :
                             (comp << 6 | jmp);
                break;
            case NUMBER:
                byte_buff |= (uint16_t) atoi(token);
                break;
            default:
                break;
        }

        // keep byte order.Must be big-endian
        byte_buff = read_msb(byte_buff);
        hfwrite(&byte_buff, sizeof(byte_buff), 1, outfp);
        // reset buff
        byte_buff &= ~byte_buff;
    }
}
