#include "cpu.h"

#include <string.h>
#include <stdlib.h>

void cpu_init(struct cpu *cpu)
{
    u8 *memory = malloc(sizeof(char) * 0xFFFFFFFF);
    if (memory == NULL) {
        exit(1);
    }
    cpu->memory = memory;
    memset(cpu->x_reg, 0, sizeof(u32) * RISCV_REG_COUNT);
}
