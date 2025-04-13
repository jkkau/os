__asm__(".code16gcc");

#include "kernel.h"

// 内核主函数
void kernel_main()
{
	char str[] = "os starting\r\n";
	print(str, 13);

	memory_check();

	enable_a20();
	init_gdt();

	while (1) {
		__asm__("hlt");
	}
}
