#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "parser.h"
#include "hdr.h"
#include "optab.h"

#define BUFF 1080


/* buffer to keep binary field */
static uint16_t byte_buff = 0x0000;

/* pass 1 */
void init_analysis(FILE *src_file, char *file_name, struct Symbol **sym_tbl) {
    char buff[BUFF], label[100];
    FILE *int_file;
    struct Symbol *sym;
    char *tk;
    tk =  buff;

    memset(label,0,100);

    // address counter for label operands
    uint16_t LC = 0x0000;

    // RAM[16]
    uint16_t RAM_ADDR = 0x0010;

    // create intermediate file
    strcat(file_name, ".int");
    int_file = hfopen(file_name, "w");

    while (fgets(buff, sizeof(buff), src_file) != NULL) {
        sscanf(buff, "%s", buff);

        // ignore comments,spaces
        if (*buff == '/' ||
            *buff == '\r' ||
            isspace(*buff) ||
            *buff == '\n')
            continue;

        int i=0;
        switch (buff[0]) {
            case '(':
                // (LOOP)

                while (*(++tk) != ')')
                    label[i++] = *tk;
                insert_symtab(sym_tbl, label, LC);
                // flush buff
                memset(buff,0,100);
                memset(label,0,100);
                // set pointer again
                tk =  buff;
                break;
            case '@':
                // @2
                if (isdigit(buff[1])) {
                    break;
                } else {
                    // @i
                    if (islower(buff[1])) {
                        sym = scan_symtab(*sym_tbl, &buff[1]);
                        if (!sym)
                            insert_symtab(sym_tbl, &buff[1], RAM_ADDR++);
                    }
                }
            default:
                break;
        }
        if (buff[0] != '\0') {
            fprintf(int_file, "%s\n", buff);
            // increase ROM address
            ++LC;
        }
    }
    hfclose(int_file);
    hfclose(src_file);
}

/* pass 2 */
void init_synthesis(char *fname, struct Symbol **sym_tbl) {
    char buff[BUFF];
    char  *token, *instr_fields[2];
    unsigned short dest, comp, jmp;
    struct Symbol *sym;
    FILE *ifile,*ofile;

    // int file
    ifile = hfopen(fname, "r");

    char *t = strchr(fname, '.');
    *t = '\0';
    strcat(fname, ".hex");
    // output file
    ofile = hfopen(fname, "wb");

    write_hdr(ofile);

    while (fgets(buff, sizeof(buff), ifile) != NULL) {
        sscanf(buff, "%s", buff);

        if (buff[0] == '@') {
            // operand parse
            if (!isdigit(buff[1])) {
                sym = scan_symtab(*sym_tbl, &buff[1]);
                if (sym) {
                    byte_buff |= sym->addr;
                }
            } else
                byte_buff = (uint16_t) atoi(&buff[1]);
        } else {
            // opcode parse
            token = strchr(&buff[0], '=');
            if (token) {
                instr_fields[0] = strtok(&buff[0], "=");
                instr_fields[1] = strtok(NULL, "=");
                dest = scan_opc(instr_fields[0], DEST);
                comp = scan_opc(instr_fields[1], COMP);

                byte_buff |= (dest << 3 | comp << 6);
            } else {
                instr_fields[0] = strtok(&buff[0], ";");
                instr_fields[1] = strtok(NULL, ";");
                comp = scan_opc(instr_fields[0], COMP);
                jmp = scan_opc(instr_fields[1], _JMP);

                byte_buff |= (comp << 6 | jmp);
            }
        }
        write_ofile(ofile);

    }
    hfclose(ifile);
    hfclose(ofile);
}

void write_ofile(FILE *ofile){
    // keep byte order.Must be big-endian
    byte_buff = read_msb(byte_buff);
    hfwrite(&byte_buff, sizeof(byte_buff), 1, ofile);
    // reset buff
    byte_buff &= ~byte_buff;

}

