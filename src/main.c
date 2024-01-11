#include "cpu.h"

int main(void)
{
    struct cpu cpu;
    cpu_init(&cpu);
    cpu_load_kernel(&cpu, "embed/bin/release/riscv_embed");
    while (1)
        cpu_run_next(&cpu);
    return 0;
}
