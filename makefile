all:
	nasm -f bin ./boot.asm -o boot.bin

show:
	ndisasm ./os.iso

qemu:
	qemu-system-x86_64 -hda ./boot.bin

clean:
	rm *.o
	rm *.bin