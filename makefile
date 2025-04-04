CC = gcc
ASM = nasm
CFLAGS = -c -m32 -fno-pie -fno-stack-protector -nostdlib -g
ASMFLAGS = -f elf32 -g -F dwarf
LDFLAGS = -m elf_i386 -Ttext 0x7e00

all: os.img

boot.elf: boot/boot.asm
	$(ASM) -f elf32 -g -F dwarf $< -o $@

boot.bin: boot.elf
	ld -m elf_i386 -Ttext 0x7c00 --oformat=binary -o $@ $<

# 自动查找 kernel 目录下的所有 .c 文件并生成对应的 .o 文件
SRCS = $(wildcard kernel/*.c)
OBJS = $(SRCS:.c=.o)

# 通用的 .c 文件编译规则
%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

start.o: kernel/start.asm
	$(ASM) $(ASMFLAGS) $< -o $@

# 使用自动生成的 .o 文件列表
kernel.elf: start.o $(OBJS)
	ld -m elf_i386 -Ttext 0x7e00 -g $^ -o $@

kernel.bin: kernel.elf
	objcopy -O binary $< $@

os.img: boot.bin kernel.bin
	# Create an empty disk image (1MB)
	dd if=/dev/zero of=$@ bs=1M count=1
	# Write the boot sector
	dd if=boot.bin of=$@ bs=512 count=1 conv=notrunc
	# Write the kernel starting at sector 2
	dd if=kernel.bin of=$@ bs=512 count=1 seek=1 conv=notrunc

debug: os.img boot.elf kernel.elf
	# qemu-system-x86_64 -hda $< -s -S
	# qemu-system-x86_64 -s -S -m 128M -drive file=os.img,index=0,media=disk,format=raw
	qemu-system-i386 -s -S -m 128M -drive file=os.img,index=0,media=disk,format=raw

gdb-boot: boot.elf
	gdb boot.elf \
		-ex "set confirm off" \
		-ex "set architecture i386:x86-64" \
		-ex "target remote localhost:1234" \
		-ex "break *0x7c00" \
		-ex "continue"

gdb-kernel: kernel.elf
	gdb kernel.elf \
		-ex "set confirm off" \
		-ex "set architecture i386:x86-64" \
		-ex "file kernel.elf" \
		-ex "target remote localhost:1234" \
		-ex "break *0x7e00" \
		-ex "continue"

show:
	ndisasm os.img

qemu: os.img
	# qemu-system-x86_64 -hda $<
	# qemu-system-x86_64 -m 128M -drive file=os.img,index=0,media=disk,format=raw
	qemu-system-i386 -m 128M -drive file=os.img,index=0,media=disk,format=raw

clean:
	rm -f *.o *.bin *.img *.elf

format:
	astyle --style=kr --indent=force-tab=4 kernel/*.c
	astyle --style=kr --indent=force-tab=4 kernel/*.h