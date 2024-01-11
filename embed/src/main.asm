.globl main

.text
main:
    addi x6, x0, 0x100
    addi x5, x0, 'T'
    sb x5, 0(x6)
    addi x5, x0, 'E'
    sb x5, 0(x6)
    addi x5, x0, 'S'
    sb x5, 0(x6)
    addi x5, x0, 'T'
    sb x5, 0(x6)
    addi x5, x0, '!'
    sb x5, 0(x6)
    addi x5, x0, 0xFF
    sb x5, 0(x6)

.data
L1: .word 27
