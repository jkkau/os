[bits 16]

extern kernel_main

global _start
_start:
    ; 切换显示模式到 80x25 文本模式
    mov ax, 0x03
    int 0x10

    jmp kernel_main
