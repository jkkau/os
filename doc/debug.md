# 使用gdb调试方法

1.在终端1启动qemu  
```
make debug
```
  
2.在终端2启动gdb调试，该命令会使用gdb attach到运行的qemu上，并将断点打在内存0x7e00处  
```
make gdb-kernel
```
  
如果想要调试boot.asm，可以使用如下命令，该命令是将断点打在0x7c00处:  
```
make gdb-boot
```
  
每次执行make gdb-xxx之前需要重新执行make debug