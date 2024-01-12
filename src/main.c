#include "cpu.h"
#include "instr.h"

#include <stdio.h>
#include <string.h>

void print_debug(struct instruction instr);

#define STEP_THROUGH

int main(void)
{
    struct instruction instr;
    struct cpu cpu;
    u8 *serial;
    char buffer[256];
    u8 previous = 0;
    u8 current = 0;

    memset(buffer, 0, 256);

    cpu_init(&cpu);
    cpu_load_kernel(&cpu, "embed/bin/riscv_bin.bin");
    
    serial = &cpu.memory[0x100];

    do {
        instr = cpu_run_next(&cpu);

#ifdef STEP_THROUGH
        print_debug(instr);
        getc(stdin);
#endif

        if (*serial != previous) {
            if (*serial == 0xFF) {
                buffer[current] = '\0';
                printf("%s\n", buffer);
                memset(buffer, 0, 256);
                current = 0;
                previous = 0;
                *serial = 0;
                continue;
            }
            previous = *serial;
            buffer[current++] = *serial;
        }
    } while (cpu.x_reg[RISCV_PC_IDX] != 0);

    return 0;
}

void print_debug(struct instruction instr)
{
    struct s_instruction s;
    struct i_instruction i;
    struct u_instruction u;
    struct r_instruction r;

    instruction_get_s(instr, &s);
    instruction_get_i(instr, &i);
    instruction_get_u(instr, &u);
    instruction_get_r(instr, &r);

    printf("R-Instruction: FUNCT7 0x%02X, RS2 0x%02X, RS1 0x%02X, FUNCT3 0x%02X, RD 0x%02X\n", r.funct7, r.rs2, r.rs1, r.funct3, r.rd);
    printf("I-Instruction: IMM_11_0 0x%02X, RS1 0x%02X, FUNCT3 0x%02X, RD 0x%02X\n", i.imm_11_0, i.rs1, i.funct3, i.rd);
    printf("S-Instruction: IMM_11_5 0x%02X, RS2 0x%02X, RS1 0x%02X, FUNCT3 0x%02X, IMM_4_0 0x%02X\n", s.imm_11_5, s.rs2, s.rs1, s.funct3, s.imm_4_0);
    printf("U-Instruction: IMM_31_12 0x%02X, RD 0x%02X\n\n", u.imm_31_12, u.rd);
}
