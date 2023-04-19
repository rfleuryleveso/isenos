C_SRCs=$(shell find src/ -type f -name '*.c')
HEADERS=$(shell find src/ -type f \( -name "*.c" -or -name "*.h" \))
OBJs=$(C_SRCs:.c=.o)
#cf. https://wiki.osdev.org/GCC_Cross-Compiler
CC=gcc
LD=ld
C_FLAGS=-W -Wall -fno-exceptions -nostartfiles -ggdb -g -ffreestanding -O2 -pedantic -Wall -Wextra -nostdlib -L./deps/



#for debug
kernel.elf: ${OBJs}
	${LD} -o $@  $^ -Tlinker.ld


%.o: %.c ${HEADERS}
	${CC} $(C_FLAGS) -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o osImg.bin *.elf
	rm -rf kernel/*.o bootloader/*.bin drivers/*.o bootloader/*.o libs/*.o apps/*.o
	rm -rf newLib/*.o newLib/*.out