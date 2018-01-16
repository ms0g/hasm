#ifndef HVM_HVM_H
#define HVM_HVM_H

#include <stdio.h>
#include <stdint.h>

#define ROM_SIZE 32768 /* 32KB */
#define RAM_SIZE 16384 /* 16KB */

struct HVMData_ {
    uint16_t comp:10;
    uint8_t dest:3;
    uint8_t jmp:3;
    int16_t A_REG:16;
    int16_t D_REG:16;
    int state;
    int pc;
};
typedef struct HVMData_ HVMData;

enum hvm_state {
    hvm_fetch,
    hvm_decode,
    hvm_execute
};

/* Memory */
uint16_t ROM[ROM_SIZE];
int16_t RAM[RAM_SIZE];

/* Initialize VM */
static void vm_init(char *arg);

/* VM State: Fetch, Decode, Execute */
static uint16_t fetch(HVMData *hdt);
static void decode(uint16_t instr, HVMData *hdt);
static void execute(HVMData *hdt);

/* Memory snapshot */
static void snapshot(HVMData *hdt);

#endif //HVM_HVM_H
