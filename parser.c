#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "parser.h"
#include "w_ob.h"


#define CLEAN_OPERAND(d, s) d=strtok(s,")(@")

#define BUFF 1080

static unsigned short LC = 0x0000; /* address counter for label operands */
static unsigned short byte_buff = 0x0000; /* buffer to keep binary field */
static unsigned short RAM_ADDR = 0x0010; /* RAM[16] */

struct C_INS _instructions[] = {
        /* Comp Mnemonics */
        {"0",   COMP, 0b1110101010000000},
        {"1",   COMP, 0b1110111111000000},
        {"-1",  COMP, 0b1110111010000000},
        {"D",   COMP, 0b1110001100000000},
        {"A",   COMP, 0b1110110000000000},
        {"!D",  COMP, 0b1110001101000000},
        {"!A",  COMP, 0b1110110001000000},
        {"-D",  COMP, 0b1110001111000000},
        {"-A",  COMP, 0b1110110011000000},
        {"D+1", COMP, 0b1110011111000000},
        {"A+1", COMP, 0b1110110111000000},
        {"D-1", COMP, 0b1110001110000000},
        {"A-1", COMP, 0b1110110010000000},
        {"D+A", COMP, 0b1110000010000000},
        {"D-A", COMP, 0b1110010011000000},
        {"A-D", COMP, 0b1110000111000000},
        {"D&A", COMP, 0b1110000000000000},
        {"D|A", COMP, 0b1110010101000000},
        {"M",   COMP, 0b1111110000000000},
        {"!M",  COMP, 0b1111110001000000},
        {"-M",  COMP, 0b1111110011000000},
        {"M+1", COMP, 0b1111110111000000},
        {"D+M", COMP, 0b1111000010000000},
        {"D-M", COMP, 0b1111010011000000},
        {"M-D", COMP, 0b1111000111000000},
        {"D&M", COMP, 0b1111000000000000},
        {"D|M", COMP, 0b1111010101000000},
        /* Dest Mnemonics */
        {"M",   DEST, 0b1110000000001000},
        {"D",   DEST, 0b1110000000010000},
        {"MD",  DEST, 0b1110000000011000},
        {"A",   DEST, 0b1110000000100000},
        {"AM",  DEST, 0b1110000000101000},
        {"AD",  DEST, 0b1110000000110000},
        {"AMD", DEST, 0b1110000000111000},
        /* Jump Mnemonics */
        {"JGT", JMP,  0b1110000000000001},
        {"JEQ", JMP,  0b1110000000000010},
        {"JGE", JMP,  0b1110000000000011},
        {"JLT", JMP,  0b1110000000000100},
        {"JNE", JMP,  0b1110000000000101},
        {"JLE", JMP,  0b1110000000000110},
        {"JMP", JMP,  0b1110000000000111},
};
#define SIZE_OF_INSTRUCTIONS (int)(sizeof(_instructions)/sizeof(_instructions[0]))

void preprocessor(FILE *src_file, char *file_name, struct Symbol **sym_tbl) {
    char buff[BUFF], clean_buff[100];
    FILE *int_file;
    char *temp;
    struct Symbol* sym;

    // create intermediate file
    int_file = fopen(file_name, "w");
    while (fgets(buff, sizeof(buff), src_file) != NULL) {
        sscanf(buff, "%s", buff);

        // ignore comments,spaces
        if (*buff == '/') continue;
        if (*buff == '\r') continue;
        if (isspace(*buff)) continue;
        if (*buff == '\n') continue;

        strcpy(clean_buff, buff);

        CLEAN_OPERAND(temp, clean_buff);
        switch (buff[0]) {
            case '(':
                // (LOOP)
                insert(sym_tbl, temp, LC);
                // After inserting label,flush buff.Because we just need addresses of labels.
                buff[0] = '\0';
                break;
            case '@':
                // @2
                if (isdigit(temp[0])) {
                    break;
                } else {
                    // @i
                    if (islower(temp[0])){
                        sym = operand_search(*sym_tbl, temp);
                        if(!sym)
                            insert(sym_tbl, temp, RAM_ADDR++);
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
    fclose(int_file);
    fclose(src_file);
}


void processor(FILE *int_file, char *file_name, struct Symbol **sym_tbl) {
    char buff[BUFF], clean_buff[100];
    char *temp, *sign, *instr_fields[2];
    unsigned short dest, comp, jmp;
    struct Symbol *sym;

    FILE *bin_file;
    bin_file = fopen(file_name, "wb");

    write_hdr(bin_file);

    while (fgets(buff, sizeof(buff), int_file) != NULL) {
        sscanf(buff, "%s", buff);
        strcpy(clean_buff, buff);

        CLEAN_OPERAND(temp, clean_buff);
        if (buff[0] == '@') {
            // operand parse
            if (!isdigit(temp[0])) {
                sym = operand_search(*sym_tbl, temp);
                if (sym) {
                    byte_buff |= sym->bin;
                }
            } else
                byte_buff = (unsigned short) atoi(temp);
        } else {
            // opcode parse
            sign = strchr(temp, '=');
            if (sign) {
                instr_fields[0] = strtok(temp, "=");
                instr_fields[1] = strtok(NULL, "=");
                dest = opcode_search(instr_fields[0], DEST);
                comp = opcode_search(instr_fields[1], COMP);
                byte_buff |= (dest | comp);
            } else {
                instr_fields[0] = strtok(temp, ";");
                instr_fields[1] = strtok(NULL, ";");
                comp = opcode_search(instr_fields[0], COMP);
                jmp = opcode_search(instr_fields[1], JMP);
                byte_buff |= (comp | jmp);
            }
        }
        // keep byte order.Must be big-endian
        byte_buff = unint16_read(byte_buff);
        fwrite(&byte_buff, sizeof(byte_buff), 1, bin_file);
        // reset buff
        byte_buff &= ~byte_buff;
    }
    fclose(bin_file);
}


unsigned short opcode_search(char *name, enum C_INS_FIELD type) {
    for (int i = 0; i < SIZE_OF_INSTRUCTIONS; ++i) {
        if ((strcmp(name, _instructions[i].name) == 0) && _instructions[i].type == type)
            return _instructions[i].opcode;
    }
    return 0;
}
