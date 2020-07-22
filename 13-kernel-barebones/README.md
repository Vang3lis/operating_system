# README

该部分正式进入整合部分，最开始确实没怎么弄懂这是怎么启动的



## os-dev.pdf

在 os-dev.pdf 中是这样介绍这一节的

The involved steps are as follows:

+ write and compile the kernel code
+ write and assemble the boot sector code
+ create a kernel image that includes not only our boot sector but our compiled kernel code
+ load out kernel code into memory
+ switch to 32-bit protected mode
+ begin executing our kernel code



在我做这一节的实验的时候，觉得在 `bootsector.asm` 中首先调用 load_kernel 进行加载 kernel 进入内存，然后通过 `10-32bit-enter/32bit-switch.asm` 进行 `lgdt`加载 gdt 表和切换到 `32-bit protected mode` ，对于 `32-bit protected mode` 的寄存器和栈进行初始化，之后会跳转至 `BEGIN_PM` 进行跳转至加载的 `kernel`



## qemu IDA调试

由于我的 gdb 调试 16位架构的时候，设置 `set architecture i8086`出了问题，我先用的IDA调试的，最后我把我的`.gdbinit`去掉了，然后利用 `x/10i $pc` 调试程序



在 IDA 中调试时，可利用 `Alt+S`设置段的架构是 `16|32`位的，并且可以 rebase 程序基址为 0x7c00 ，运行到最后同样是利用 `x` 指令打印 `0xb8000` 是否为`X`



## 编译运行指令

`gcc -m32 -ffreestanding -fno-pie -c kernel.c -o kernel.o`

`nasm kernel_entry.asm -f elf -o kernel_entry.o`

`ld -m elf_i386 -o kernel.bin -Ttext 0x1000 kernel_entry.o kernel.o --oformat binary`

`nasm bootsect.asm -f bin -o bootsect.bin`

`cat bootsect.bin kernel.bin > os-image`



调试指令 `qemu_system_i386/x86_64 -s -S -fda ./os-image`

IDA 就设置端口 attach 就行， gdb `target remote:1234`