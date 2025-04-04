__asm__(".code16gcc");

#include "kernel.h"

// 内核主函数
void kernel_main()
{
	char str[] = "os starting\r\n";
	print(str, 13);

	while (1)
	{
		__asm__("hlt");
	}
}
