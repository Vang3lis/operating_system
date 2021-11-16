# README

## 需求

```
Goal: Fix miscellaneous issues with our code
```

目标就是重新修订一下代码

### boot

对于`kernel_entry.asm`的文件，添加入口点`_start`的定义，以及为了区分关键词`main`，将`main`修改为`kernel_main`

### cpu

修改数据结构，把自定义的 `u8 u16 u32` 改为用 `<stdint.h>` 定义的 `uint8_t` 等，并且将其中的所有这些 `define` 的类型的地方均进行更正

同理 `kernel | libc` 部分也进行修改

### interrupt handlers 

前期的知识回顾：

`cli`：禁止中断发生 

`sti`：允许中断发生

`ISRs` 全称为 `Interrupt Service Routines` 翻译为中断服务例程，在写 `isr_install` 时，原来的代码中会存在`cli`

`IRQs` 全称为 `interrupt requests` 中断请求

`IDT` 是中断向量表

---

渣翻，勿喷：

`cli` is redundant, since we already established on the IDT entries if interrupts are enabled within a handler using the `idt_gate_t` flags.

提到说 `cli` 是多余的，因为如果中断可以使用`idt_gate_t`标志位，那说明我们已经建立了IDT表项

`sti` is also redundant, as `iret` loads the eflags value from the stack, which contains a bit telling whether interrupts are on or off. In other words the interrupt handler automatically restores interrupts whether or not interrupts were enabled before this interrupt

提到说 `sti` 时多余的，因为 `iret` 从堆栈中加载 eflags 值，其中包含一些说明中断是打开还是关闭的信息，换句话说，无论中断在此中断之前是否启用，中断处理程序都会自动恢复中断

On cpu/isr.h, struct registers_t has a couple issues. First, the alleged `esp` is renamed to `useless`. The value is useless because it has to do with the current stack context, not what was interrupted. Then, we rename useresp to esp

提到说在`cpu/isr.h` 中我们对于原`esp`命名为`unless`，因为这个值是无用的因为这个值是当前栈帧的上下文，而与中断无关，因此我们重新将`useresp`改回`esp` 

We add `cld` just before call `isr_handler` on `cpu/interrupt.asm` as suggested by the osdev wiki.

在 `call isr_handler` 之前加入了 `cld` 汇编指令（`cld` 指令是用于清空 `eflags` 中的 `DF` 方向标志位的 `direction Flags`）

There is a final, important issue with `cpu/interrupt.asm`. The common stubs create an instance of `struct registers` on the stack and then call the C handler. But that breaks the ABI, since the stack belongs to the called function and they may change them as they please. It is needed to pass the struct as a pointer.

提到最后且最重要一个`issue`就是`cpu/interrupt.asm`代码中的一个，the common stubs创建了一个栈上的`struct registers`的实例然后调用了`C handler`，但是这样是违反了`ABI`，因为堆栈是属于被调用函数的，并且被调用函数可以随意修改它们，因此需要是传递的是结构体指针

---
P.S. `ABI` 为 `Application Binary Interface`, [In computer software, an application binary interface (ABI) is an interface between two binary program modules](https://www.wikiwand.com/en/Application_binary_interface)，[我们把符号修饰标准、变量内存布局、函数调用方式等这些跟可执行代码二进制兼容性相关的内容称为ABI(Application Binary Interface)](https://www.zhihu.com/question/381069847)。

我认为这里完全就是一个编程规范的问题

---

To achieve this, edit `cpu/isr.h` and `cpu/isr.c` and change `registers_t r` into `registers_t *t`, then, instead of accessing the fields of the struct via `.`, access the fields of the pointer via `->`. Finally, in `cpu/interrupt.asm`, and add a push esp before calling both isr_handler and irq_handler -- remember to also pop eax to clear the pointer afterwards.

为了满足这些要求，就把 `registers_t r` 改为 `registers_t *t` ，并且对于 `cpu/interrupt.asm` 的汇编代码进行修改，之前对其进行 `push esp` 传参，并在最后 `pop` 进行平衡堆栈

Both current callbacks, the timer and the keyboard, also need to be changed to use a pointer to registers_t.

同时对于现有的 `callbacks`，例如 `timer` 和 `keyboard` 中，都需要改成指针 `regsiters_t`

---
P.S.

经过调试，可以看到这两个 `esp` 的区别，一个是中断前的值（从记录的`eip`可知），一个是产生中断后的内核中的值。（不过我不知道为什么原来的`kernel.elf`没这个符号，我自己在`debug_struct`文件夹中重新生成了这个符号才行的）

```assembly
[----------------------------------registers-----------------------------------]
EAX: 0x10 --> 0xf000ff53 --> 0x0 (0xf000ff53)
EBX: 0x7d99 ("Started in the 32-bit Protected Mode")
ECX: 0x2 --> 0xff53f000 --> 0x0 --> 0xf000ff53 (0x00000000)
EDX: 0x40 --> 0xc0005634 --> 0x0 --> 0xf000ff53 (0x00000000)
ESI: 0x0 --> 0xf000ff53 (0x00000000)
EDI: 0x0 --> 0xf000ff53 (0x00000000)
EBP: 0x8ffc0 --> 0x8ffe0 --> 0x8fff0 --> 0x90000 --> 0x0 --> 0xf000ff53 (0x00000000)
ESP: 0x8ff5c --> 0x8ff60 --> 0x10 --> 0xf000ff53 --> 0x0 (0xf000ff53)
EIP: 0x11e9 --> 0x703e8 --> 0x0 --> 0xf000ff53 (0x00000000)
EFLAGS: 0x16 (carry PARITY ADJUST zero sign trap interrupt direction overflow)
[-------------------------------------code-------------------------------------]
   0x11e4 <irq_common_stub+13>:	mov    fs,eax
   0x11e6 <irq_common_stub+15>:	mov    gs,eax
   0x11e8 <irq_common_stub+17>:	push   esp
=> 0x11e9 <irq_common_stub+18>:	call   0x18f1 <irq_handler>
   0x11ee <irq_common_stub+23>:	pop    eax
   0x11ef <irq_common_stub+24>:	pop    ebx
   0x11f0 <irq_common_stub+25>:	mov    ds,ebx
   0x11f2 <irq_common_stub+27>:	mov    es,ebx
No argument
[------------------------------------stack-------------------------------------]
0000| 0x8ff5c --> 0x8ff60 --> 0x10 --> 0xf000ff53 --> 0x0 (0xf000ff53)
0004| 0x8ff60 --> 0x10 --> 0xf000ff53 --> 0x0 (0xf000ff53)
0008| 0x8ff64 --> 0x0 --> 0xf000ff53 (0x00000000)
0012| 0x8ff68 --> 0x0 --> 0xf000ff53 (0x00000000)
0016| 0x8ff6c --> 0x8ffc0 --> 0x8ffe0 --> 0x8fff0 --> 0x90000 --> 0x0 (--> ...)
0020| 0x8ff70 --> 0x8ff84 --> 0x20 --> 0xf000fea5 --> 0x0 --> 0xf000ff53 (0x00000000)
0024| 0x8ff74 --> 0x7d99 ("Started in the 32-bit Protected Mode")
0028| 0x8ff78 --> 0x40 --> 0xc0005634 --> 0x0 --> 0xf000ff53 (0x00000000)
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value
0x000011e9 in irq_common_stub ()
gdb-peda$ p *(struct registers_t *)0x8ff60
No struct type named registers_t.
gdb-peda$ add-symbol-file ./debug_struct/struct
add symbol table from file "./debug_struct/struct"
Reading symbols from ./debug_struct/struct...
gdb-peda$ p *(struct registers_t *)0x8ff60
$1 = {
  ds = 0x10,
  edi = 0x0,
  esi = 0x0,
  ebp = 0x8ffc0,
  unless = 0x8ff84,
  ebx = 0x7d99,
  edx = 0x40,
  ecx = 0x2,
  eax = 0x5d,
  int_no = 0x20,
  err_code = 0x0,
  eip = 0x1aa9,
  cs = 0x8,
  eflags = 0x216,
  esp = 0x40,
  ss = 0x5d
}
gdb-peda$ x/10i 0x1aa9
   0x1aa9 <init_timer+107>:	add    esp,0x10
   0x1aac <init_timer+110>:	nop
   0x1aad <init_timer+111>:	leave  
   0x1aae <init_timer+112>:	ret 
```

这个地方可以回顾一下原来的 `registers_t` 结构体，可以看到最下面的结构体是属于`processor`自动添加的

```c
typedef struct {
	uint32_t ds; // Data segment selector
	uint32_t edi, esi, ebp, unless, ebx, edx, ecx, eax; // Pushed by pusha
	uint32_t int_no, err_code; // Interrupt number and error code (if applicable)
	uint32_t eip, cs, eflags, esp, ss; // Pushed by the processor automatically
} registers_t;
```

如有不清楚，解释如下 [what-are-the-registers-pushed-to-the-stack-when-an-interrupt-occurs](https://stackoverflow.com/questions/38292995/what-are-the-registers-pushed-to-the-stack-when-an-interrupt-occurs)

---


