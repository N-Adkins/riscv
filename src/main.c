#include "cpu.h"

int main(void)
{
    struct cpu cpu;
    cpu_init(&cpu);
    cpu_run_next(&cpu);
    return 0;
}
