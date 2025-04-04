[bits 16]

_start:
	mov ax, 0
	mov es, ax

read_disk:
	; bx = 0x7e00 读取到的内存地址
	mov bx, 0x7e00
	; ch = 0 磁道号
	; cl = 2 扇区号，扇区号1是MBR，就是boot.asm代码所在的位置. 扇区号2是kernel.bin(start.asm + C代码)所在的位置
	mov cx, 0x0002
	; al = 1 读取一个扇区
	; ah = 0x02 读取扇区
	mov ax, 0x0208
	; dh = 0 磁头号
	; dl = 80H 表示硬盘
	mov dx, 0x80
	; int 0x13 中断，读取硬盘
	int 0x13
	jc read_disk

	; 读取成功则跳转到0x7e00处执行
	jmp 0x7e00


times 510 - ($ - $$) db 0
dw 0xAA55
