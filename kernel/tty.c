__asm__(".code16gcc");

#include "tty.h"

void print(char *str, int len)
{
	int i = 0;
	while (i < len) {
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