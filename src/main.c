#include "cpu.h"

#include <stdio.h>

int main(void)
{
    u8 *serial;
    u8 previous;
    struct cpu cpu;

    cpu_init(&cpu);
    cpu_load_kernel(&cpu, "embed/bin/release/riscv_embed");

    serial = &cpu.memory[0x1000000];
    previous = *serial;

    do {
        cpu_run_next(&cpu);

        if (*serial != previous) {
            printf("%c", *serial);
            previous = *serial;
        }
    } while (cpu.x_reg[RISCV_PC_IDX] != 0);

    return 0;
}
