__asm__(".code16gcc");

#include "kernel.h"

void print1(char *str, int len)
{
	int i = 0;
	while (i < len)
	{
		unsigned char c = str[i++];
		__asm__ __volatile__(

			"movb %b0, %%al\n"
			"movb $0x0E, %%ah\n"
			"xor %%bh, %%bh\n"
			"int $0x10\n"
			:
			: "q"(c)
			: "ax", "bx");
	}
}

// 内核主函数
void kernel_main()
{
	char str[] = "hello";
	print1(str, 5);

	while (1)
	{
		__asm__("hlt");
	}
}
