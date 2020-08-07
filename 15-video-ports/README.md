# README

## 需求

本部分仅用来表述阅读代码和阅读 os-dev.pdf 所得到的一些编写 I/O 代码的需求以及 screen 打印的约定。

在 os-dev.pdf 的 6.1 节中有如下一段话

```
By writing to the screen we have actually already encountered a friendlier form of hardware I/O, known as memory-mapped I/O, whereby data written directly to a certain address  range  in  main  memory  is  written  to  the  device’s  internal  memory buffer,  but now it is time to understand more about this interaction between CPU and hardware.
```

其中提到 `a friendly form of  hardware I/O, know as memory-mapped I/O`这种打印到屏幕上的`I/O`是一种映射到内存的`I/O`，因此只要在内存的某个地址上写上需要打印的内容即可显示在屏幕中。

并且其中提到两个关键的汇编指令 `in/out` 其中 `in: input from port | out: output to port`

`in` 的时候只需要设置`port` 和接收`result`

`out`的时候需要设置`port` 和 `data`

```
intel
__asm__("in al, dx": "=a" (result): "d" (port))
__asm__("out dx, al": : "a" (data), "d" (port))

at&t
__asm__("in %%dx, %%al": "=a" (result): "d" (port))
__asm__("out %%al, %%dx": : "a" (data), "d" (port))
```

同时 `al` 可扩展成 `ax`，从一个字节扩展成两个字节



```
out(port = 0x3d4, data = 14)  1. Ask for high byte of the cursor offset (data 14)
high = in(port = 0x3d5) 可获得光标的高一个字节
out(port = 0x3d4, data = 15)  2. Ask for low byte (data 15)
low = in(port = 0x3d5) 可获得光标的低一个字节

position = 2 * ((high<<8) + low)
```

其中这样得出的数 乘2 的原因为：这个 video memory 的每个字符单元都是由两个字节构成的，一个是表示字符的ASCII码，一个是属性字节（用来设置字符单元的颜色 `#define WHITE_ON_BLACK 0x0f`）

另外前面提到只需要向内存上的某个地址写上打印的内容即可，这个地址为 `0xb8000`

如下所示即可打印一个 `X`在屏幕上

```
char* vga=0xb8000;
vga[position] = 'X';
vga[position+1] = 0x0f;
```

该文件夹的需求即为打印一个 `X`在屏幕上



## Makefile

最初不明白为什么会按照如下方式写 `Makefile`

```
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) --ffreestanding -c $< -o $@
```

这个 `%.o` 和 `%.c` 是一起编译吗，多个 `.c` 文件生成一个 `.o` 文件？ 并且为什么用的是 `$<` ？`$<`不是代表着`first dependence`吗？后来在用 `make --just-print` 的时候找到了答案

```shell
nasm boot/kernel_entry.asm -f elf -o boot/kernel_entry.o
gcc -m32 -g -fno-pie -ffreestanding -c kernel/kernel.c -o kernel/kernel.o
gcc -m32 -g -fno-pie -ffreestanding -c drivers/ports.c -o drivers/ports.o
ld -m elf_i386 -o kernel.bin -Ttext 0x1000 boot/kernel_entry.o kernel/kernel.o drivers/ports.o --oformat binary
cat boot/bootsect.bin kernel.bin > os-image
```

可以看到 `.c` 和 `.o` 是一个一个编译的，可以联想到 `Makefile` 的一个机制，如果 `target file`生成了则不再生成（实际上有要求`target file`生成的时间是晚于 `dependence` ），于是可以不停匹配 `%.c` 将其生成为一个 `%.o` 



另外就是 `debug` 部分，最开始写的时候是 `gdb -ex "command1" -ex "command2"... ` ，觉得这样写实在繁琐，不能把指令合成在一条语句里面吗？搜寻 `gdb`的文档可得：

```
-ex command
    Execute a single GDB command.

    This option may be used multiple times to call multiple commands. It may also be interleaved with ‘-command’ as required.

    gdb -ex 'target sim' -ex 'load' \
   -x setbreakpoints -ex 'run' a.out
```

并在官方文档中找到了一个简单的解决该问题的办法 `-x file`

```
-command file
-x file
    Execute commands from file file. The contents of this file is evaluated exactly as the source command would. See Command files.
```

最后在 `Makefile` 中可以用 `gdb -x ./script` 的方式解决该问题

> <https://sourceware.org/gdb/current/onlinedocs/gdb/File-Options.html>
