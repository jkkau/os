__asm__(".code16gcc");
#include "kernel.h"

memory_info_t memory_info;

/**
memory_raw_t *memory_raw = &memory_info.raws[index];
    memory_info.raws 是存储 内存信息的结构体数组，index用于遍历这些条目

int 0x15：
输入("a"(sign16), "b"(index), "c"(MEMORY_MAX_COUNT), "d"(sign32), "D"(memory_raw)):
    EAX = 0xE820：告诉BIOS调用E820功能，查看内存信息
    EBX = index：查询索引，每次查询会递增。没有更多的内存信息时会被BIOS置为0
    ECX = MEMORY_MAX_COUNT：指定我们希望获取的最大内存条目大小，需要大于等于20以符合E820规范
    EDX = 0x534D4150 ("SMAP")：数字签名，BIOS需要返回相同的值来确认请求有效
    EDI = memory_raw：BIOS将返回的内存信息存储到 memory_raw 指向的地址，实际时写入[ES:Di]指向的地址中
输出("=a"(signature), "=c"(bytes), "=b"(index)):
    EAX = 0x534D4150   : 签名 ("SMAP")
    ECX = 返回的结构体大小
    EBX = 递增后的index : 用于下一次调用，没有更多的内存信息时会被BIOS置为0
*/
void memory_check()
{
    // char str[] = "checking memory\r\n";
    // print(str, 17);

    int sign32 = 0x534D4150, sign16 = 0xE820;
    int index = 0, signature, bytes;
    while (1)
    {
        memory_raw_t *memory_raw = &memory_info.raws[index];
        __asm__ __volatile__("int $0x15"
                             : "=a"(signature), "=c"(bytes), "=b"(index)
                             : "a"(sign16), "b"(index), "c"(MEMORY_MAX_COUNT), "d"(sign32), "D"(memory_raw));
        if (signature != sign32)
        {
            char str[] = "memory check error!\r\n";
            print(str, 21);
        }
        memory_info.count++;
        if (index == 0)
        {
            char str[] = "memory check success!\r\n";
            print(str, 23);
            break;
        }
    }
}