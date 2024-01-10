#ifndef RISCV_SRC_INSTR_H
#define RISCV_SRC_INSTR_H

#include "types.h"

struct instruction {
    u32 raw;     
};

struct r_instruction {
    u8 rd;
    u8 funct3;
    u8 rs1;
    u8 rs2;
    u8 funct7;
};

struct i_instruction {
    u8 rd;
    u8 funct3;
    u8 rs1;
    i16 imm_11_0;
};

struct s_instruction {
    i8 imm_4_0;
    u8 funct3;
    u8 rs1;
    u8 rs2;
    i8 imm_11_5;
};

struct u_instruction {
    u8 rd;
    i32 imm_31_12;
};

u8 instruction_get_op(struct instruction instr);
void instruction_get_r(struct instruction instr, struct r_instruction *r_instr);
void instruction_get_i(struct instruction instr, struct i_instruction *i_instr);
void instruction_get_s(struct instruction instr, struct s_instruction *s_instr);
void instruction_get_u(struct instruction instr, struct u_instruction *u_instr);

#endif
