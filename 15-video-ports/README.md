# README

在这一章节 Makefile 编写的时候还是出现了一些问题



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