C_SOURCES = ${wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c}
HEADERS = ${wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h}
OBJ = ${C_SOURCES:.c=.o cpu/interrupt.o}

CC = i386-elf-gcc
GDB = i386-elf-gdb 
LD = i386-elf-ld
CFLAGS = -g -masm=intel -std=c99 

all: run

os-image.bin: boot/bootsect.bin kernel.bin
	cat $^ > $@

kernel.bin: boot/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: boot/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^

run: os-image.bin 
	qemu-system-i386 -fda $<

debug: os-image.bin kernel.elf
	qemu-system-i386 -s -S -fda $< &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

%.o : %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o : %.asm
	nasm -felf $< -o $@

%.bin : %.asm
	nasm -fbin $< -o $@

clean:
	rm -rf *.bin *.elf *.o
	rm -rf kernel/*.o
	rm -rf libc/*.o 
	rm -rf drivers/*.o
	rm -rf cpu/*.o 
	rm -rf boot/*.bin boot/*.o