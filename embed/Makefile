all:
	@mkdir -p bin
	riscv32-unknown-elf-as -c src/main.asm -o bin/main.o
	riscv32-unknown-elf-gcc -nostdlib -T src/main.ld -o bin/riscv_bin.elf bin/main.o
	riscv32-unknown-elf-objcopy -O binary bin/riscv_bin.elf bin/riscv_bin.bin
