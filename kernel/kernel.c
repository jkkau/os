#include "kernel.h"

// 内核主函数
void kernel_main()
{
	// print("Hello, world!\n", 0xFF);
	// char *str = "Hello, world!\n";
	// static char str[] = "Hello, world!\n";
	const char *const str = "Hello, world!\n";
	int i = 0;
	for (char *p = str; *p != '\0' && i < 5; p++, i++) {
		unsigned char c = *p;
		__asm__ __volatile__(

			"movb %b0, %%al\n"
			"movb $0x0E, %%ah\n"
			"xor %%bh, %%bh\n"
			"int $0x10\n"
			:
			: "q"(c)
			: "ax", "bx");
	}

	while (1) {
		__asm__("hlt");
	}
}
