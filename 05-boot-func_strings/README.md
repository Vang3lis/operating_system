写 `boot_sect_print_hex` 还是踩了很多坑的

+ 想用`dx`中的`dh`和`dl`直接分开高32位和低32位，后来在`mov`或者`add`的时候都出错了，可能是汇编学的不好，最后我是用 `shl | shr` 进行分开的 （想过用 and 分开，后来看教材，确实是用 and 写的）
+ `mov edx, [ebx]` 这个最初我是写的 `mov edx, byte [ebx]` 不知道为什么这个汇编指令是不行的
+ 对于平衡堆栈这边，开头是 `push ecx` 结尾是 `pop, cx` 导致过堆栈不平衡
+ 就是，由于编译之后发现 `main` 函数之后就跟着`print`和`print_hex`的代码，不知道为什么`qemu`每次打印出来的个数都不一致 
  + fix 当时忘记写 `jmp $` 进行循环了
+ gdb 的汇编是不可信的，因为是 16 bits，因此 ida 的汇编是准确的


