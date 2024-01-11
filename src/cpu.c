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

void cpu_load_kernel(struct cpu *cpu, const char *file)
{
    FILE *file_ptr;
    long len;
    
    file_ptr = fopen(file, "rb");
    if (!file_ptr) {
        printf("Kernel loading error -> Failed to find kernel binary %s\n", file);
        return;
    }

    fseek(file_ptr, 0, SEEK_END);
    len = ftell(file_ptr);
    rewind(file_ptr);

    if (len > RISCV_MEM_MAX) {
        printf("Provided kernel is too large!");
        goto CLEANUP;
    }

    fread(cpu->memory, len, 1, file_ptr);

CLEANUP:;
    fclose(file_ptr);
}

void cpu_run_next(struct cpu *cpu)
{
    struct instruction instr;
    struct r_instruction r_instr;
    struct i_instruction i_instr;
    struct s_instruction s_instr;
    struct u_instruction u_instr;
    RISCV_XLEN pc;
    u8 op;
    u8 op_map;

    (void)u_instr;
    
    pc = cpu->x_reg[RISCV_PC_IDX];
    if (pc + 3 > RISCV_MEM_MAX - 1) { /* Addressing safety check */
        exit(1);
    }

    /* 
     * This can be done faster with type punning and abusing host system endianness but
     * I'm not doing that unless I have to
     */
    instr.raw = ((RISCV_XLEN)cpu->memory[pc + 3]) | (((RISCV_XLEN)(cpu->memory[pc + 2])) << 8) | (((RISCV_XLEN)cpu->memory[pc + 1]) << 16) | 
        (((RISCV_XLEN)cpu->memory[pc]) << 24);
    
    op = instruction_get_op(instr);
    op_map = op >> 2;

    /* printf("Executing 0x%02X\n", instr.raw); */

    switch(op_map) {
    
    case RISCV_MAP_OP_IMM: {
        instruction_get_i(instr, &i_instr);

        switch (i_instr.funct3) {

        case RISCV_INSTR_ADDI:
            if (i_instr.rd == 0 && i_instr.imm_11_0 == 0 && i_instr.rs1 == 0) {
                /* NOP */
                break;
            }
            cpu->x_reg[i_instr.rd] = i_instr.imm_11_0 + cpu->x_reg[i_instr.rs1];
            break;

        case RISCV_INSTR_SLTI:
            cpu->x_reg[i_instr.rd] = ((i32)cpu->x_reg[i_instr.rs1]) < i_instr.imm_11_0;
            break;

        case RISCV_INSTR_SLTIU:
            cpu->x_reg[i_instr.rd] = cpu->x_reg[i_instr.rs1] < ((u32)i_instr.imm_11_0);
            break;

        case RISCV_INSTR_XORI:
            cpu->x_reg[i_instr.rd] = cpu->x_reg[i_instr.rs1] ^ i_instr.imm_11_0;
            break;

        case RISCV_INSTR_ORI:
            cpu->x_reg[i_instr.rd] = cpu->x_reg[i_instr.rs1] | i_instr.imm_11_0;
            break;
        
        case RISCV_INSTR_ANDI:
            cpu->x_reg[i_instr.rd] = cpu->x_reg[i_instr.rs1] & i_instr.imm_11_0;
            break;

        default:
            /* printf("Unhandled OP_IMM function 0x%02X\n", i_instr.funct3); */
            break;
        }

        break;
    }

    case RISCV_MAP_OP: {
        instruction_get_r(instr, &r_instr);

        switch (r_instr.funct3) {
        
        case RISCV_INSTR_ADD_SUB:
            if (r_instr.funct7) {
                /* ADD */
                cpu->x_reg[r_instr.rd] = cpu->x_reg[r_instr.rs1] + cpu->x_reg[r_instr.rs2];
            } else {
                /* SUB */
                cpu->x_reg[r_instr.rd] = cpu->x_reg[r_instr.rs1] - cpu->x_reg[r_instr.rs2];
            }
            break;

        case RISCV_INSTR_SLL:
            cpu->x_reg[r_instr.rs1] <<= (cpu->x_reg[r_instr.rs2] & 0x1F);
            break;

        case RISCV_INSTR_SLT:
            cpu->x_reg[r_instr.rd] = ((i32)((cpu->x_reg[r_instr.rs1] & 0x10) << 27)) < ((i32)((cpu->x_reg[r_instr.rs2] & 0x10) << 27));
            break;

        case RISCV_INSTR_SLTU:
            cpu->x_reg[r_instr.rd] = cpu->x_reg[r_instr.rs1] < cpu->x_reg[r_instr.rs2];
            break;

        case RISCV_INSTR_XOR:
            cpu->x_reg[r_instr.rd] = cpu->x_reg[r_instr.rs1] ^ cpu->x_reg[r_instr.rs2];
            break;

        case RISCV_INSTR_SRL_SRA:
            if (r_instr.funct7) {
                /* SRL */ 
                cpu->x_reg[r_instr.rd] = cpu->x_reg[r_instr.rs1] << cpu->x_reg[r_instr.rs2];
            } else {
                /* SRA */
                cpu->x_reg[r_instr.rd] = ((i32)cpu->x_reg[r_instr.rs1]) >> cpu->x_reg[r_instr.rs2];
            }
            break;

        case RISCV_INSTR_OR:
            cpu->x_reg[r_instr.rd] = cpu->x_reg[r_instr.rs1] | cpu->x_reg[r_instr.rs2];
            break;

        case RISCV_INSTR_AND:
            cpu->x_reg[r_instr.rd] = cpu->x_reg[r_instr.rs1] & cpu->x_reg[r_instr.rs2];
            break;
        
        default:
            /* printf("Unhandled OP funct3 0x%02X w/ funct7 0x%02X\n", r_instr.funct3, r_instr.funct7); */
            break;
        }

        break;
    }

    case RISCV_MAP_LOAD: {
        instruction_get_i(instr, &i_instr);
        
        switch (i_instr.funct3) {
            
        case RISCV_INSTR_LB: {
            u32 value = cpu->memory[i_instr.imm_11_0 + i_instr.rs1];
            cpu->x_reg[i_instr.rd] = value | ((value >> 7) << 31);
            break;
        }

        case RISCV_INSTR_LH: {
            u32 index = i_instr.imm_11_0 + i_instr.rs1; 
            u32 value = cpu->memory[index] | (cpu->memory[index + 1] << 8);
            cpu->x_reg[i_instr.rd] = value | ((value >> 15) << 31);
            break;
        }

        case RISCV_INSTR_LW: 
            cpu->x_reg[i_instr.rd] = cpu->memory[i_instr.imm_11_0 + i_instr.rs1];
            break;

        case RISCV_INSTR_LBU:
            cpu->x_reg[i_instr.rd] = cpu->memory[i_instr.imm_11_0 + i_instr.rs1];
            break;

        case RISCV_INSTR_LHU: {
            u32 index = i_instr.imm_11_0 + i_instr.rs1; 
            u32 value = cpu->memory[index] | (cpu->memory[index + 1] << 8);
            cpu->x_reg[i_instr.rd] = value;
            break;
        }

        default:
            /* printf("Unhandled LOAD funct3 0x%02X\n", i_instr.funct3); */
            break;
        }

        break;
    }

    case RISCV_MAP_STORE: {
        instruction_get_s(instr, &s_instr);

        switch (s_instr.funct3) {
        
        case RISCV_INSTR_SB:
            cpu->memory[s_instr.imm_11_5 + cpu->x_reg[s_instr.rs1]] = cpu->x_reg[s_instr.rs2] & 0xFF;
            break;

        case RISCV_INSTR_SH: {
            u32 value = cpu->x_reg[s_instr.rs2];
            u32 index = s_instr.imm_11_5 + cpu->x_reg[s_instr.rs1];
            cpu->memory[index + 1] = value & 0xFF00;
            cpu->memory[index] = value & 0xFF;
            break;
        }

        case RISCV_INSTR_SW: {
            u32 value = cpu->x_reg[s_instr.rs2];
            u32 index = s_instr.imm_11_5 + cpu->x_reg[s_instr.rs1];
            cpu->memory[index + 3] = value & 0xFF000000;
            cpu->memory[index + 2] = value & 0xFF0000;
            cpu->memory[index + 1] = value & 0xFF00;
            cpu->memory[index] = value & 0xFF;
            break;
        }

        default:
            /* printf("Unhandled STORE funct3 0x%02X\n", i_instr.funct3); */
            break;
        }

        break;
    }

    default:
        /* printf("Unhandled opcode 0x%02X\n", op); */
        break;
    }

    cpu->x_reg[RISCV_PC_IDX] += 4;
}
