[bits 16]

global _start

extern kernel_main

_start:
    jmp kernel_main
