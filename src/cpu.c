#include "cpu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instr.h"

void cpu_init(struct cpu *cpu)
{
    u8 *memory = malloc(sizeof(char) * RISCV_MEM_MAX);
    if (memory == NULL) {
        printf("Malloc failed -> RISCV requires >4GB memory!");
        exit(1);
    }
    memset(memory, 0, sizeof(char) * RISCV_MEM_MAX);

    cpu->memory = memory;
    memset(cpu->x_reg, 0, sizeof(u32) * RISCV_REG_COUNT);
}

void cpu_run_next(struct cpu *cpu)
{
    u32 pc;
    u8 op;
    struct instruction instr;

    pc = cpu->x_reg[RISCV_PC_IDX];
    if (pc + 3 > RISCV_MEM_MAX - 1) { /* Addressing safety check */
        exit(1);
    }

    /* 
     * This can be done faster with type punning and abusing host system endianness but
     * I'm not doing that unless I have to
     */
    instr.raw = ((u32)cpu->memory[pc + 3]) | ((u32)(cpu->memory[pc + 2]) << 8) | (((u32)cpu->memory[pc + 1]) << 16) | 
        ((u32)cpu->memory[pc] << 24);
    
    op = instruction_get_op(instr);
    switch(op) {
    
    default:
        printf("Unhandled opcode 0x%02X\n", op);
        break;
    }
}
