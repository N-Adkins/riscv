#include "instr.h"

u8 instruction_get_op(struct instruction instr)
{
    return instr.raw & 0x7;
}

void instruction_get_r(struct instruction instr, struct r_instruction *r_instr)
{
    r_instr->rd = (instr.raw >> 7) & 0x1F;
    r_instr->funct3 = (instr.raw >> 12) & 0x7;
    r_instr->rs1 = (instr.raw >> 15) & 0x1F;
    r_instr->rs2 = (instr.raw >> 20) & 0x1F;
    r_instr->funct7 = (instr.raw >> 25) & 0x7;
}

void instruction_get_i(struct instruction instr, struct i_instruction *i_instr)
{
    i_instr->rd = (instr.raw >> 7) & 0x1F;
    i_instr->funct3 = (instr.raw >> 12) & 0x7;
    i_instr->rs1 = (instr.raw >> 15) & 0x1F;
    i_instr->imm_11_0 = ((instr.raw >> 20) & 0xFFF) | (instr.raw & 0x80000000);
}

void instruction_get_s(struct instruction instr, struct s_instruction *s_instr)
{
    s_instr->imm_4_0 = ((instr.raw >> 7) & 0xF) | ((instr.raw & 0x10) << 26);
    s_instr->funct3 = (instr.raw >> 12) & 0x7;
    s_instr->rs1 = (instr.raw >> 15) & 0x1F;
    s_instr->rs2 = (instr.raw >> 20) & 0x1F;
    s_instr->imm_11_5 = ((instr.raw >> 25) & 0x3F) | (instr.raw & 0x80000000);
}

void instruction_get_u(struct instruction instr, struct u_instruction *u_instr)
{
    u_instr->rd = (instr.raw >> 7) & 0x1F;
    u_instr->imm_31_12 = ((instr.raw >> 12) & 0xFFFFF) | (instr.raw & 0x80000000);
}
