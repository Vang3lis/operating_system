# README

## 需求

```
Goal: Implement our first IRQ handlers: the CPU timer and the keyboard
```

目标就是实现一个`IRQ`的处理：`CPU timer` 和 `keyboard`

### CPU timer

主要就是在 `cpu/` 下实现 `time.h` 和 `time.c`

这个里面主要实现的时候，就是注册中断处理，然后每次时间中断时，在`timer_callback`函数中就要实现打印一个递增的全局变量 `tick` 的值 （这个的部分功能在 `screen.h` 中实现了接口，可以直接拿来用）

```cpp
void init_timer(u32 freq)
{
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT value: hardware clock at 1193180 Hz */
    u32 divisor = 1193180 / freq;
    u8 low = (u8)(divisor & 0xff);
    u8 high = (u8)((divisor >> 8) & 0xff);

    /* set the command */
    port_byte_out(0x43, 0x36);
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}
```

然后在注册完之后需要按照一个频率去设置值，就是代码的下面部分

### keyboard

这个是在 `drivers/` 下实现 `keyboard.h` 和 `keyboard.c`

主要逻辑，还是注册中断处理

在中断处理函数中，需要接收 `keyboard` 传来的一个 `scancode` 值，然后根据这个 `scancode` 的值打印对应的动作是什么

我只照着板子实现了个最简单的

整个的 [ScanCode](https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html) 可以在这里面看到

### kernel.c main()

最后在 `kernel.c` 中开放中断请求，以及对于这些 `init_xxx` 进行调用，当然可以注释 `init_timer` 以便更好的看到 `keyboard` 的值

```cpp
void main()
{
	clear_screen();
	isr_install();
	
	// enable interrupts again
	__asm__ __volatile__ ("sti");
	init_timer(50);
	init_keyboard();
}
```