#include "tty.h"

void print(char *str, uint8_t color)
{
	for (char *p = str; *p != '\0'; p++) {
		// char c = (*p & 0xFF) | 0X0E00;
		char c = 'a';
		__asm__ __volatile__ ("mov $0xE, %%ah\n"
							  "mov %[c], %%al\n\t"
							  "int $0x10\n"
							  :: [c]"r"(c), "b"(color));
	}
}