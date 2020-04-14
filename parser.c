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
static u16 byte_buff;

/* address counter for label operands */
static u16 LC;

/* RAM[16] */
static u16 RAM_ADDR = 0x0010;


/* pass 1 */
void init_analysis(FILE *infp, char *file_name, struct Symbol **sym_table) {
    char buff[BUFF], token[100];
    struct Symbol *sym;
    FILE *outfp;
    int tok_type;

    memset(token, 0, 100);

    outfp = hasm_fopen(file_name, "w");

    while (fgets(buff, sizeof(buff), infp) != NULL) {
        sscanf(buff, "%s", buff);

        if (tokenize(buff, token, &tok_type, NULL, pass1) < 0)
            continue;

        switch (tok_type) {
            case LABEL:
                insert_symtab(sym_table, token, LC);
                break;
            case A_INS:
                sym = scan_symtab(*sym_table, token);
                if (!sym) {
                    insert_symtab(sym_table, token, RAM_ADDR++);
                }
                break;
            case C_INS:
            default:
                break;
        }

        // flush buffer
        memset(token, 0, 100);

        if (tok_type != LABEL) {
            fprintf(outfp, "%s\n", buff);
            // increase ROM address
            ++LC;
        }
    }
    hasm_fclose(outfp);
}

/* pass 2 */
void init_synthesis(FILE *infp, FILE *outfp, struct Symbol **sym_table) {
    char buff[BUFF], token[100];
    unsigned short dest, comp, jmp;
    struct Symbol *sym;
    int tok_type;

    while (fgets(buff, sizeof(buff), infp) != NULL) {
        C_INS_t _inst = {.comp="", .dest="", .jmp=""};

        sscanf(buff, "%s", buff);

        tokenize(buff, token, &tok_type, &_inst, pass2);

        switch (tok_type) {
            case A_INS:
                sym = scan_symtab(*sym_table, token);
                if (sym) {
                    byte_buff |= sym->addr;
                }
                break;
            case C_INS:
                dest = scan_opc(_inst.dest, hasm_dest);
                comp = scan_opc(_inst.comp, hasm_comp);
                jmp = scan_opc(_inst.jmp, hasm_jmp);
                if (dest && comp) {
                    byte_buff |= dest << 3 | comp << 6;
                } else
                    byte_buff |= comp << 6 | jmp;
                break;
            case NUMBER:
                byte_buff |= (u16) atoi(token);
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
