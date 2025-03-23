#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "types.h"
#include "tty.h"

#define MEMORY_MAX_COUNT 24

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
