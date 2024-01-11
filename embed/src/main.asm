.globl main

.text
main:
    addi x6, x0, 0x100
    addi x5, x0, 0x5A
    sb x5, 0(x6)
    addi x5, x0, 0x41
    sb x5, 0(x6)
    addi x5, x0, 0x4D
    sb x5, 0(x6)
    addi x5, x0, 0x4E
    sb x5, 0(x6)
    addi x5, x0, 0x21
    sb x5, 0(x6)
    addi x5, x0, 0xFF
    sb x5, 0(x6)

.data
L1: .word 27
