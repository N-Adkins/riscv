#include "cpu.h"

#include <stdio.h>
#include <string.h>

int main(void)
{
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
        cpu_run_next(&cpu);

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
