C_SRCs=$(shell find src/ -type f -name '*.c')
ASM_SRCs=$(shell find src/ -type f -name '*.asm')
HEADERS=$(shell find src/ -type f \( -name "*.c" -or -name "*.h" \))

OBJs=$(C_SRCs:.c=.o src/gdt/gdt_asm.o src/idt/interrupt_wrapper.o)
#cf. https://wiki.osdev.org/GCC_Cross-Compiler
CC=gcc
LD=ld
C_FLAGS=-W -Wall -fno-exceptions -nostartfiles -ggdb -g -ffreestanding -O2 -pedantic -Wall -Wextra -nostdlib -L./deps/
LD_FLAGS=--no-relax


#for debug
kernel.elf: ${OBJs}
	${LD} $(LD_FLAGS) -o $@  $^ -Tlinker.ld


%.o: %.c ${HEADERS}
	${CC} $(C_FLAGS) -c $< -o $@

%.o: %.asm
	nasm $< -f elf64 -o $@


clean:
	find ./src -type f -name '*.o' -delete