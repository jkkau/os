#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "types.h"
#include "tty.h"

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
typedef struct {
	uint32_t base_l;
	uint32_t base_h;
	uint32_t length_l;
	uint32_t length_h;
	uint32_t type;
} memory_raw_t;

typedef struct {
	memory_raw_t raws[MEMORY_MAX_COUNT];
	uint32_t count;
} memory_info_t;

extern memory_info_t memory_info;

void memory_check();

memory_info_t *get_memory_info();

#endif // _KERNEL_H_
