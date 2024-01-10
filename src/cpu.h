#ifndef RISCV_SRC_CPU_H
#define RISCV_SRC_CPU_H

#include "types.h"

#define RISCV_XLEN 32
#define RISCV_REG_COUNT 33
#define RISCV_PC_IDX 32
#define RISCV_MEM_MAX 0xFFFFFFFF

#define RISCV_OP_IMM
#define RISCV

struct cpu {
    u32 x_reg[RISCV_REG_COUNT];
    u8 *memory;
};

void cpu_init(struct cpu *cpu);
void cpu_run_next(struct cpu *cpu);

#endif
