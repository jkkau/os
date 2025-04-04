# 遇到的问题记录

## 进入C代码之后print函数打印不出来字符
未设置char *str = "xxx"的存储位置导致str指向无效地址,  
使用char str[] = "xxx"将str存储在栈上来规避。  

## 将print函数定义在kernel.c中,可以成功打印。但print函数放到tty.c中,在kernel.c中调用tty.c的print函数,打印不了字符
原因是kernel.c使用__asm__(".code16gcc")指示编译器生成16位汇编代码,用于实模式。  
而tty.c没有使用,导致编译器编译这两个文件时的不对齐,在tty.c中加入__asm__(".code16gcc")解决该问题。  

## C函数执行过程中奇怪的打印问题  
见commit: https://github.com/jkkau/os/commit/798461ede22e16bd3cef326f51665c5ec279b82e  
在该commit中，kernel_main已经将memory_check注释掉，但是"os starting"字符串仍然打印不出来,  
当如commit: https://github.com/jkkau/os/commit/ff2edbb9a6c141d058bb62da2a7173a6605d8694 所做的,  
把memory_check函数中的前两行print代码删除掉，"os starting"可以打印出来了。  
值得注意的是，当我将memory_check的前两行代码删除，在kernel_main中将memory_check的注释放开后，是可以成功打印  
os starting  
memory check success!  
的，看上去只有在memory_check函数开头的位置申请栈空间(char str[] = "checking memory\r\n"有问题, char str[]="a\n"没问题)才会出现打印问题。  
我有尝试在start.asm中让ss=0, sp=0x7c00或者sp=0x9000，以将栈指针指向0x7c00和0x9000，来正确的设置C语言栈，但是都没有解决问题。  

紧接着遇到了另一个问题: 
在commit https://github.com/jkkau/os/commit/d5ec940d80cc6c331c89c944739b4e2f56b25012中,  
memory.c中函数get_memory_info的实现是注释掉的，如果将注释放开，memory_check函数执行不了，通过gdb调试发现，  
代码执行到kernel_main的print("os starting")之后，执行memory_check时显示"Cannot find bounds of current function".  

通过objdump查看kernel.elf的.text大小，发现包含get_memory_info的程序.text大小时0x206 bytes，大于512 bytes.  
不包含get_memory_info的程序.text大小是0x1f7 bytes, 刚刚小于512 bytes. 
所以问题的根本原因是boot.asm只读了一个扇区(512 bytes)的kernel.elf文件到0x7e00处，多余的代码并没有被加载到内存中。  

没有屏蔽get_memory_info编译出来kernel.elf
```
jkkau@DESKTOP-2SOU9GE:/mnt/d/work/os(master)$ objdump -h kernel.elf

kernel.elf:     file format elf32-i386

Sections:
Idx Name          Size      VMA       LMA       File off  Algn
  0 .text         00000206  00007e00  00007e00  00000e00  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .eh_frame     000000a8  00009000  00009000  00002000  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  2 .bss          000001e4  0000b000  0000b000  00003000  2**5
                  ALLOC
  3 .comment      0000002b  00000000  00000000  000020a8  2**0
                  CONTENTS, READONLY
  4 .debug_aranges 00000080  00000000  00000000  000020d3  2**0
                  CONTENTS, READONLY, DEBUGGING, OCTETS
  5 .debug_info   000003a5  00000000  00000000  00002153  2**0
                  CONTENTS, READONLY, DEBUGGING, OCTETS
  6 .debug_abbrev 0000027e  00000000  00000000  000024f8  2**0
                  CONTENTS, READONLY, DEBUGGING, OCTETS
  7 .debug_line   0000019c  00000000  00000000  00002776  2**0
                  CONTENTS, READONLY, DEBUGGING, OCTETS
  8 .debug_str    00000170  00000000  00000000  00002912  2**0
                  CONTENTS, READONLY, DEBUGGING, OCTETS
  9 .debug_line_str 0000005a  00000000  00000000  00002a82  2**0
                  CONTENTS, READONLY, DEBUGGING, OCTETS
```  
屏蔽get_memory_info编译出来kernel.elf  
```
jkkau@DESKTOP-2SOU9GE:/mnt/d/work/os(master)$ objdump -h kernel.elf

kernel.elf:     file format elf32-i386

Sections:
Idx Name          Size      VMA       LMA       File off  Algn
  0 .text         000001f7  00007e00  00007e00  00000e00  2**4
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .eh_frame     00000088  00008000  00008000  00001000  2**2
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  2 .bss          000001e4  0000a000  0000a000  00002000  2**5
                  ALLOC
  3 .comment      0000002b  00000000  00000000  00001088  2**0
                  CONTENTS, READONLY
  4 .debug_aranges 00000080  00000000  00000000  000010b3  2**0
                  CONTENTS, READONLY, DEBUGGING, OCTETS
  5 .debug_info   0000038a  00000000  00000000  00001133  2**0
                  CONTENTS, READONLY, DEBUGGING, OCTETS
  6 .debug_abbrev 00000265  00000000  00000000  000014bd  2**0
                  CONTENTS, READONLY, DEBUGGING, OCTETS
  7 .debug_line   00000195  00000000  00000000  00001722  2**0
                  CONTENTS, READONLY, DEBUGGING, OCTETS
  8 .debug_str    0000016c  00000000  00000000  000018b7  2**0
                  CONTENTS, READONLY, DEBUGGING, OCTETS
  9 .debug_line_str 0000005a  00000000  00000000  00001a23  2**0
                  CONTENTS, READONLY, DEBUGGING, OCTETS
```

## 通过vscode使用gdb调试时报错
报错信息: unable to start debugging. Unexpected GDB output from command "-target-select remote 127.0.0.1:1234". Remote 'g' packet reply is too long (expected 312 bytes, got 608 bytes)  
原因: 使用qemu-system-x86_64启动进程，而gdb期望的是32位架构(makefile中通过-m32编译的)  
将qemu-system-x86_64改为qemu-system-i386即可  