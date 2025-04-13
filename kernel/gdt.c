__asm__(".code16gcc");

#include "kernel.h"

/**
gdt表3个表项作用:
0	空描述符（NULL）	必须是全 0，处理器用作保留项
1	内核代码段	定义内核执行代码所在的段，权限 Ring 0，可执行只读
2	内核数据段	定义内核读写数据所在的段，权限 Ring 0，可读写
*/
gdt_table_t gdt_table[3];

static void set_gdt_entry(int i, uint32_t base, uint32_t limit, uint16_t attr)
{
    gdt_table[i].limit_l = limit & 0xFFFF;
    gdt_table[i].base_l = base & 0xFFFF;
    gdt_table[i].base_m = (base >> 16) & 0xFF;
    gdt_table[i].attr = attr | ((limit >> 16) & 0x0F); // 粒度 + 高4位 limit
    gdt_table[i].base_h = (base >> 24) & 0xFF;
}

void init_gdt()
{
    // 空段
    set_gdt_entry(0, 0, 0, 0);

    // 代码段：基址=0，大小=4GB，权限=0x9A（代码段，可执行，读，存在）
    set_gdt_entry(1, 0x00000000, 0xFFFFF, 0x9A | 0xC00); // 0xC00 为 4KB 粒度 + 32 位模式

    // 数据段：基址=0，大小=4GB，权限=0x92（数据段，可读写，存在）
    set_gdt_entry(2, 0x00000000, 0xFFFFF, 0x92 | 0xC00);

    // 加载 GDT
    lgdt((uint32_t)gdt_table, sizeof(gdt_table));

    char str[] = "GDT init success!\r\n";
    print(str, 19);
}