[bits 16]

extern kernel_main

global _start
_start:
    ; 清空屏幕
    mov ax, 0x03
    int 0x10

    jmp kernel_main
