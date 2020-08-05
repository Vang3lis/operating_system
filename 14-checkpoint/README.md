# README

这个文件夹纯属用于整理所有需要的代码到一个文件夹中，而不是像 13 文件夹中用`"../05-boot-func_string/boot_sect_print.asm"` 这种形式写 `bootsect.asm` 代码



## Makefile 知识

### shell

shell 函数用来执行 shell 命令

`$(shell echo 1234)`



### wildcard

wildcard 函数用来在 Makefile 中，替换 Bash 的通配符

`C_SOURCES = $(wildcard kernel/*.c)`



### subst

subst函数用来文本替换，格式如下

`$(subst from, to, text)`

`$(subst ee, EE, feet on the street)`



### patsubst

oatsubst 函数用于模式匹配的替换，格式如下

`$(patsub pattern, replacement, text)`

`$(patsubst %.c, %.o, c.c bar.c)`



### 替换后缀名

替换后缀名函数的写法是：变量名+冒号+后缀名替换规则。实际上它是patsubst函数的一种简写形式

`min: $(OUTPUT:.js=.min.js)`

把变量OUTPUT中的后缀名为 .js 的名字全部替换成 .min.js 的名字



> <https://www.ruanyifeng.com/blog/2015/02/make.html>



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

查阅资料：值得注意的是，每行命令在一个单独的 shell 中执行。这些 shell 之间没有继承关系，唯一解决的办法是将两行指令写在一行（用分隔符隔开），或者在换行符前利用反斜杠转义，或者加上 `.ONESHELL:` 指令