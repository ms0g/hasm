#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "lexer.h"

#define BUFF 1080

/* buffer to keep binary field */
static u16 byte_buff;

/* address counter for label operands */
static u16 LC;

/* RAM[16] */
static u16 RAM_ADDR = 0x0010;

static char buff[BUFF];
static char token[100];
static struct Symbol *sym;

#define FLUSH() { \
    memset(buff, 0, BUFF);\
    memset(token, 0, 100);\
}

/* pass 1 */
void init_analysis(FILE *infp, char *file_name, struct Symbol **sym_table) {
    FILE *outfp;
    int tok_type;
    
    FLUSH()
    
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
                if (sym == NULL) {
                    insert_symtab(sym_table, token, RAM_ADDR++);
                }
                break;
            case C_INS:
            default:
                break;
        }
        
        if (tok_type != LABEL) {
            fprintf(outfp, "%s\n", buff);
            // increase ROM address
            ++LC;
        }
        
        // flush buffer
        FLUSH()
    }
    hasm_fclose(outfp);
    sym = NULL;
}

/* pass 2 */
void init_synthesis(FILE *infp, FILE *outfp, struct Symbol **sym_table) {
    unsigned short dest, comp, jmp;
    int tok_type;
    
    FLUSH()
    
    while (fgets(buff, sizeof(buff), infp) != NULL) {
        C_INS_t _inst;
        memset(&_inst, 0, sizeof(C_INS_t));
        
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
                dest = c_inst_dest_get(&_inst);
                comp = c_inst_comp_get(&_inst);
                jmp = c_inst_jmp_get(&_inst);
                
                if (dest && comp) {
                    byte_buff |= dest << 3 | comp << 6;
                } else {
                    byte_buff |= comp << 6 | jmp;
                }
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
        
        // flush buff
        byte_buff &= ~byte_buff;
        FLUSH()
        
        
        Hasm_Free(_inst.comp)
        Hasm_Free(_inst.dest)
        Hasm_Free(_inst.jmp)
    }
}
