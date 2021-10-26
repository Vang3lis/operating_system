# README

## 需求

```
Goal: Finish the interrupts implementation and CPU timer
```

目标就是完善中断的实现以及CPU timer

### 基础知识

> There are several methods for communicating with external devices. Two of the most useful and popular are polling and interrupting.

> Polling: Spin in a loop, occasionally checking if the device is ready.

> Interrupts: Do lots of useful stuff. When the device is ready it will cause a CPU interrupt, causing your handler to be run.

`IRQs` 全称为 `interrupt requests` 中断请求

`PIT` 全称为 `programmable interval timer` 可编程间隔定时器

`Polling` 轮询

`EOI` 全称为 `End of Interrupt` 每次中断处理结束


只有用了 `PIT` 我们才可以在计算机中计时，`PIT`会根据你设定的 `interval` ，每隔 `interval` 时间就会发送一个0号中断

### PIC 的设定

对于 `PIC` 的设定，一般如下所示

```c
/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */
 
#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */
 
/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void PIC_remap(int offset1, int offset2)
{
	unsigned char a1, a2;
 
	a1 = inb(PIC1_DATA);                        // save masks
	a2 = inb(PIC2_DATA);
 
	outb(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
 
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
 
	outb(PIC1_DATA, a1);   // restore saved masks.
	outb(PIC2_DATA, a2);
}
```

就是一个硬件上的初始化设置，反正我是直接抄的板子，可以看到 `Master` 是 `IRQ: 0-7`，`interrupt numbers: 0x8-0xf`, `Slave` 是 `IRQ: 8-15`，`interrupt numbers: 0x70-0x77` ，映射到 `ISR` 上面是 `0x20-0x27` 和 `0x28-0x2f` 刚好对应上面的两个 `offset`

### 与 18-interrupts 的比较

添加了 `IRQs` 的支持

`irq_common_stub` 汇编上存在一些差异

`isr_install` 的时候需要初始化 `Master | Slave` 

添加 `register_interrupt_handler` 和 `irq_handler` ，以及 `irq_handler` 中对于 `EOI` 情况下对于板子的设定

## 参考

### PIC 资料

> https://web.archive.org/web/20160327010849/http://www.jamesmolloy.co.uk/tutorial_html/5.-IRQs%20and%20the%20PIT.html
> https://wiki.osdev.org/PIC
> https://zhuanlan.zhihu.com/p/26464793
> https://www.jianshu.com/p/16bf6aaa1ade

### PIT 理解

> https://www.cnblogs.com/bitzhuwei/p/os-in-30-days-10-programmable-interval-timer.html
> http://airtrack.me/posts/2015/05/05/操作系统实现（三）：中断/

## 问题

好像暂时还没体现出来时间中断的作用，感觉可能是下一章才会有窗口上的反应