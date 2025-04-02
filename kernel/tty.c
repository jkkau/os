#include "tty.h"

void print(char *str, uint8_t color)
{
	int i = 0;
	char a[] = "hello";
	while (i < 5)
	{
		unsigned char c = a[i++];
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