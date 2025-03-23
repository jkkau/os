[bits 16]

global _start

extern kernel_main

_start:
    ; 清空屏幕
    mov ax, 0x03
    int 0x10

    jmp kernel_main
