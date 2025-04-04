# 遇到的问题记录

## 进入C代码之后print函数打印不出来字符
未设置char *str = "xxx"的存储位置导致str指向无效地址,  
使用char str[] = "xxx"将str存储在栈上来规避。  

## 将print函数定义在kernel.c中,可以成功打印。但print函数放到tty.c中,在kernel.c中调用tty.c的print函数,打印不了字符
原因是kernel.c使用__asm__(".code16gcc")指示编译器生成16位汇编代码,用于实模式。  
而tty.c没有使用,导致编译器编译这两个文件时的不对齐,在tty.c中加入__asm__(".code16gcc")解决该问题。  
