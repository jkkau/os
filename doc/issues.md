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
该问题目前没有解决，只能通过删除char str[] = "checking memory\r\n"规避掉。  
  
## 通过vscode使用gdb调试时报错
报错信息: unable to start debugging. Unexpected GDB output from command "-target-select remote 127.0.0.1:1234". Remote 'g' packet reply is too long (expected 312 bytes, got 608 bytes)  
原因: 使用qemu-system-x86_64启动进程，而gdb期望的是32位架构(makefile中通过-m32编译的)  
将qemu-system-x86_64改为qemu-system-i386即可  