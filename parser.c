#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "parser.h"
#include "hdr.h"
#include "optab.h"
#include "lexer.h"
#include "utils.h"

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
    
    int_file = hasm_fopen(file_name, "w");

    while (fgets(buff, sizeof(buff), srcfp) != NULL) {
        sscanf(buff, "%s", buff);

        if (tokenize(buff, token, &tok_type, NULL, pass1) < 0)
            continue;

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
    hasm_fclose(int_file);
}

/* pass 2 */
void init_synthesis(char *file_name, struct Symbol **sym_tbl) {
    char buff[BUFF], token[100];
    unsigned short dest, comp, jmp;
    struct Symbol *sym;
    int tok_type;

    // int file
    intfp = hasm_fopen(file_name, "r");

    while (fgets(buff, sizeof(buff), intfp) != NULL) {
        C_ins_t _inst = {.comp="", .dest="", .jmp=""};

        sscanf(buff, "%s", buff);

        tokenize(buff, token, &tok_type, &_inst, pass2);

        switch (tok_type) {
            case A_INS:
                sym = scan_symtab(*sym_tbl, token);
                byte_buff |= (sym) ? sym->addr : 0;
                break;
            case C_INS:
                dest = scan_opc(_inst.dest, hasm_dest);
                comp = scan_opc(_inst.comp, hasm_comp);
                jmp = scan_opc(_inst.jmp, hasm_jmp);

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
        hasm_fwrite(&byte_buff, sizeof(byte_buff), 1, outfp);
        // reset buff
        byte_buff &= ~byte_buff;
    }
}
