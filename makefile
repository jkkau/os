CC = gcc
ASM = nasm
CFLAGS = -c -m32 -fno-pie -fno-stack-protector -nostdlib
ASMFLAGS = -f elf32
LDFLAGS = -m elf_i386 -Ttext 0x7e00

all: os.img

boot.elf: boot/boot.asm
	$(ASM) -f elf32 $< -o $@

boot.bin: boot.elf
	ld -m elf_i386 -Ttext 0x7c00 --oformat=binary -o $@ $<

kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) $< -o $@

start.o: kernel/start.asm
	$(ASM) $(ASMFLAGS) $< -o $@

kernel.bin: start.o kernel.o
	ld $(LDFLAGS) $^ -o $@

os.img: boot.bin kernel.bin
	# Create an empty disk image (10MB)
	dd if=/dev/zero of=$@ bs=1M count=1
	# Write the boot sector
	dd if=boot.bin of=$@ bs=512 count=1 conv=notrunc
	# Write the kernel starting at sector 2
	dd if=kernel.bin of=$@ bs=512 seek=1 conv=notrunc

show:
	ndisasm os.img

qemu: os.img
	qemu-system-x86_64 -hda $<

clean:
	rm -f *.o *.bin *.img *.elf