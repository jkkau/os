__asm__(".code16gcc");

#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "types.h"
#include "tty.h"

// MEMORY CHECK START
#define MEMORY_MAX_COUNT 24

/*
base_l 和 base_h: 内存区域的起始地址。base_h 是高 32 位，base_l 是低 32 位，合起来是一个 64 位地址。
length_l 和 length_h: 内存区域的大小，单位是字节，同样是 64 位表示。
type: 内存区域类型 ->
1 = 可用内存（可用来分配）
2 = 保留（不能使用）
3 = ACPI 可恢复内存
4 = ACPI NVS
5 = 坏内存（不能使用）

我们编写的是32位系统，base_h和length_h的值是0.
*/
typedef struct
{
	uint32_t base_l;
	uint32_t base_h;
	uint32_t length_l;
	uint32_t length_h;
	uint32_t type;
} memory_raw_t;

typedef struct
{
	memory_raw_t raws[MEMORY_MAX_COUNT];
	uint32_t count;
} memory_info_t;

void memory_check();

memory_info_t *get_memory_info();
// MEMORY CHECK END

// GDT START
typedef struct gdt_table_t
{
	uint16_t limit_l; // 段界限低16位
	uint16_t base_l;  // 段基址低16位
	uint8_t base_m;	  // 段基址中8位
	uint16_t attr;	  // 访问权限 + 粒度和段界限高4位
	uint8_t base_h;	  // 段基址高8位
} __attribute__((packed)) gdt_table_t;

static inline void lgdt(uint32_t start, uint32_t size)
{
	struct
	{
		uint16_t limit;
		uint16_t start_l;
		uint16_t start_h;
	} gdt;

	gdt.start_h = start >> 16;
	gdt.start_l = start & 0xFFFF;
	gdt.limit = size - 1;
	__asm__ volatile("lgdt %[g]" ::[g] "m"(gdt));
}

void init_gdt();
// GDT END

static inline void outb(unsigned short port, unsigned char value)
{
	__asm__ __volatile__("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline unsigned char inb(unsigned short port)
{
	unsigned char ret;
	__asm__ __volatile__("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

static inline void cli()
{
	__asm__ volatile("cli");
}

static inline void sti()
{
	__asm__ volatile("sti");
}

static inline void enable_a20()
{
	unsigned char port_a;

	port_a = inb(0x92); // 读取端口 0x92
	port_a |= 0x02;		// 设置 A20 使能位（bit 1）
	port_a &= ~0x01;	// 清除 bit 0，防止重启
	outb(0x92, port_a); // 写回端口
}

#endif // _KERNEL_H_
