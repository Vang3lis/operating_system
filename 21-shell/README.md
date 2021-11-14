# README

## 需求

```
Goal: Clean the code a bit and parse user input
```

目标就是清理一下代码，然后可以简单的解析用户输入（暂时做的就是可以`echo`用户输入）

### boot

`boot` 文件夹没变化

### cpu

将`ports.c`和`ports.h`移入该文件夹

在`isr.c`中实现`irq_install`，注册`IRQ0`和`IRQ1`，时间中断处理和键盘中断处理

```c
void irq_install()
{
	asm volatile("sti");
	init_timer(50);
	init_keyboard();
}
```

在`timer.c`中去除打印的内容

### drivers

对于`keyboard.c`中的注册键盘中断处理进行修改：

+ `BACKSPACE`时，`mem.c`上的`void backspace(input_buffer)`，对于输入的内容删除一格，和`screen.c`上的`kprint_backspace()`，对于屏幕上的打印，将上一格删除，并移回打印的光标

+ `ENTER`时，直接打印刚刚的用户输入

### kernel

实现`main`函数，对于`isr|irq`注册，以及实现`user_input`函数的逻辑


### libc

清理`kernel/`文件夹下面的`util.h`和`util.c`，新建文件夹`libc/`，将原文件分开为`mem.h`和`string.h`

`mem/` 文件夹主要实现`memcpy`和`memset`

```c
// mem.h
void memcpy(char* source, char* dest, int lenth);
void memset(char* dest, char value, int lenth);
```

`string.h` 主要新实现如下功能 
 
```c
// string.h
void int_to_ascii(int n, char* str);
int strlen(char* s);
int strcmp(char* s1, char* s2);
void backspace(char* s);        // 删除用户输入buffer中的一个字符
void add(char* s, char ch);     // 给用户输入buffer中添加一个字符
```

