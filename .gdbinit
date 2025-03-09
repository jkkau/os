# 设置Intel汇编语法
set disassembly-flavor intel

# 显示一些有用的信息
define hook-stop
  # 显示下一条要执行的指令
  x/i $pc
  # 显示寄存器
  info registers
end

# 用于显示当前栈内容的命令
define stack
  x/24wx $esp
end

# 用于在实模式下计算段地址的命令
define seg2linear
  print/x ($arg0 * 16 + $arg1)
end 