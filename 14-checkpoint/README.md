# README

这个文件夹纯属用于整理所有需要的代码到一个文件夹中，而不是像 13 文件夹中用`"../05-boot-func_string/boot_sect_print.asm"` 这种形式写 `bootsect.asm` 代码



## 问题

其中遇到了两个问题

一是编写 `Makefile` 的时候，`bootsect.asm` 文件中的 `include` 需要按照 `Makefile` 作为当前路径，而不是 `bootsect.asm` 的文件位置为当前路径

二是对于 `Makefile`中的`debug` 调试编写的时候，最初编写的时候如下

```shell
command 1: qemu xxxx 
command 2: peda // 在我电脑中peda指令为.gdbinit切换成peda的，因为在16bits下只有peda可set i8086
command 3: gdb -ex "xxx"
```

但是这样写会导致这个指令是满足严格先后顺序的，最后改为 `comm1 & comm2 & comm3` 即可