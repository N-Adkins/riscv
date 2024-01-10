#include "instr.h"

u8 instruction_get_op(struct instruction instr)
{
    return instr.raw & 0x7;
}

void instruction_get_r(struct instruction instr, u8 *rd, u8 *funct3, u8 *rs1, u8 *rs2, u8 *funct7)
{
    *rd = (instr.raw >> 7) & 0x1F;
    *funct3 = (instr.raw >> 12) & 0x7;
    *rs1 = (instr.raw >> 15) & 0x1F;
    *rs2 = (instr.raw >> 20) & 0x1F;
    *funct7 = (instr.raw >> 25) & 0x7;
}

void instruction_get_i(struct instruction instr, u8 *rd, u8 *funct3, u8 *rs1, i16 *imm_11_0)
{
    *rd = (instr.raw >> 7) & 0x1F;
    *funct3 = (instr.raw >> 12) & 0x7;
    *rs1 = (instr.raw >> 15) & 0x1F;
    *imm_11_0 = ((instr.raw >> 20) & 0xFFF) | (instr.raw & 0x80000000);
}

void instruction_get_s(struct instruction instr, i8 *imm_4_0, u8 *funct3, u8 *rs1, u8 *rs2, i8 *imm_11_5)
{
    *imm_4_0 = ((instr.raw >> 7) & 0xF) | ((instr.raw & 0x10) << 26);
    *funct3 = (instr.raw >> 12) & 0x7;
    *rs1 = (instr.raw >> 15) & 0x1F;
    *rs2 = (instr.raw >> 20) & 0x1F;
    *imm_11_5 = ((instr.raw >> 25) & 0x3F) | (instr.raw & 0x80000000);
}

void instruction_get_u(struct instruction instr, u8 *rd, i32 *imm_31_12)
{
    *rd = (instr.raw >> 7) & 0x1F;
    *imm_31_12 = ((instr.raw >> 12) & 0xFFFFF) | (instr.raw & 0x80000000);
}
