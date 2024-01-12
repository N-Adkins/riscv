#ifndef RISCV_SRC_CPU_H
#define RISCV_SRC_CPU_H

#include "instr.h"
#include "types.h"

typedef uint32_t RISCV_XLEN;

#define RISCV_REG_COUNT 33
#define RISCV_PC_IDX 32
#define RISCV_MEM_MAX 0xFFFFFFFF

#define RISCV_MAP_LOAD 0x0
#define RISCV_MAP_LOAD_FP 0x1
#define RISCV_MAP_MISC_MEM 0x3
#define RISCV_MAP_OP_IMM 0x4
#define RISCV_MAP_AUIPC 0x5
#define RISCV_MAP_OP_IMM_32 0x6
#define RISCV_MAP_STORE 0x8
#define RISCV_MAP_STORE_FP 0x9
#define RISCV_MAP_AMO 0x11
#define RISCV_MAP_OP 0x12
#define RISCV_MAP_LUI 0x13
#define RISCV_MAP_OP_32 0x14
#define RISCV_MAP_MADD 0x16
#define RISCV_MAP_MSUB 0x17
#define RISCV_MAP_NMSUB 0x18
#define RISCV_MAP_NMADD 0x20
#define RISCV_MAP_OP_FP 0x21
#define RISCV_MAP_OP_BRANCH 0x24
#define RISCV_MAP_OP_JALR 0x25
#define RISCV_MAP_OP_JAL 0x27
#define RISCV_MAP_OP_SYSTEM 0x28

#define RISCV_INSTR_ADDI 0x0
#define RISCV_INSTR_SLTI 0x2
#define RISCV_INSTR_SLTIU 0x3
#define RISCV_INSTR_XORI 0x4
#define RISCV_INSTR_ORI 0x6
#define RISCV_INSTR_ANDI 0x7

#define RISCV_INSTR_ADD_SUB 0x0
#define RISCV_INSTR_SLL 0x1
#define RISCV_INSTR_SLT 0x2
#define RISCV_INSTR_SLTU 0x3
#define RISCV_INSTR_XOR 0x4
#define RISCV_INSTR_SRL_SRA 0x5
#define RISCV_INSTR_OR 0x6
#define RISCV_INSTR_AND 0x7

#define RISCV_INSTR_LB 0x0
#define RISCV_INSTR_LH 0x1
#define RISCV_INSTR_LW 0x2
#define RISCV_INSTR_LBU 0x4
#define RISCV_INSTR_LHU 0x5

#define RISCV_INSTR_SB 0x0
#define RISCV_INSTR_SH 0x1
#define RISCV_INSTR_SW 0x2

struct cpu {
    RISCV_XLEN x_reg[RISCV_REG_COUNT];
    u8 *memory;
};

void cpu_init(struct cpu *cpu);
void cpu_load_kernel(struct cpu *cpu, const char *file);
struct instruction cpu_run_next(struct cpu *cpu);

#endif
