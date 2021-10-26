# README

## 需求

```
Goal: Set up the Interrupt Descriptor Table to handle CPU interrupts
```

目标就是建立处理终端的机制，在`int $num`时能跳转到处理中断的地方，并打印相应的中断原因。

### 基础知识


> The GDT and the IDT are descriptor tables. They are arrays of flags and bit values describing the operation of either the segmentation system (in the case of the GDT), or the interrupt vector table (IDT).

> Stands for "Interrupt Service Routine." An ISR (also called an interrupt handler) is a software process invoked by an interrupt request from a hardware device. It handles the request and sends it to the CPU, interrupting the active process. When the ISR is complete, the process is resumed.


`GDT` 是全局符号描述表
`IDT` 是中断向量表
`ISRs` 全称为 `Interrupt Service Routines` 翻译为中断服务例程

#### idt_gate_t 结构体

```c
typedef struct {
	u16 low_offset; // lower 16bits of handler function address
	u16 sel; // kernel segment selector
	u8  always0;
	/* flag:
	 * +------+-+---+-----+
	 * |bits  |7|6 5| 4-0 |
	 * +------+-+---+-----+
	 * |denote|P|DPL|01110|
	 * +------+-+---+-----+
	 * 
	 * Bit 7: "Interrupt is present"
	 * Bits 6-5: Privilege level of caller (0=ring0 kernel 3=ring3 user)
	 * Bit 4: Set to 0 for interrupt gates
	 * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate"
	 */
	u8 flags;
	u16 high_offset; // higher/upper 16 bits of handler function address
} __attribute__((packed)) idt_gate_t;
```

```
every interrupt gate (handler) is defined
```

存储一个中断的信息的结构体

一般都为 256 个中断

```
idt_gate_t idt[256];
```



#### idt_register_t 结构体

```c
typedef struct {
	u16 limit; // (The size of idt[] array) - 1
	u32 base; // The address of the first element in out idt[] array.
} __attribute__((packed)) idt_register_t;
```

```
A pointer to the array of interrupt handlers.
```

一个指向全部中断处理的结构体的指针结构

一般都在 `lidt [eax]` 中使用，与 `lgdt [gdt_descriptor]`一致

```
idt_register_t idt_reg;
```



#### registers_t 结构体

```c
/* Struct which aggregates many registers */
typedef struct {
	u32 ds; // Data segment selector
	u32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
	u32 int_no, err_code; // Interrupt number and error code (if applicable)
	u32 eip, cs, eflags, useresp, ss; // Pushed by the processor automatically
} registers_t;
```

这个结构体就是存储调用中断时的所有上下文的状态信息，该结构体作为 `isr_handler(registers_t)` 的参数



#### 建立机制

对于每一个 `idt[idx]` 进行填写好内容，然后在 `idt_reg`中填好内容。

因此加载`kernel`的最初的时候需要先按照上述说的建立中断映射表，然后再进行 `int $num`进行测试



#### 汇编指令

```asm
pusha:
push eax
push ecx
push edx
push ebx
push esp
push ebp
push esi
push edi
```

`pusha` 和 `popa`恰好对立

`cli`：禁止中断发生

`sti`：允许中断发生

在中断的过程中可以使用 `cli` 和 `sti`保护程序正常进行

```assembly
iret:
pop eip (=ret)
pop cs
pop eflags
(pop esp, pop ss)
```

目前打括号的原因是我查询的资料说明，`iret`是`pop eip, cs, eflags, esp, ss`但是从调试来看并非如此。



####  debug

```assembly
 ► 0x12ba <main+27>        int    2
 
00:0000│ esp  0x8ffe0 —▸ 0x90000 —▸ 0 —▸ 0xf000ff53 ◂— 0
01:0004│      0x8ffe4 —▸ 0x8fff8 —▸ 0x7d76 —▸ 0x5300feeb ◂— 0
02:0008│ ebp  0x8ffe8 —▸ 0x90000 —▸ 0 —▸ 0xf000ff53 ◂— 0
03:000c│      0x8ffec —▸ 0x1005 —▸ 0x8955feeb —▸ 0 ◂— 0xf000ff53
04:0010│      0x8fff0 —▸ 2 —▸ 0xff53f000 —▸ 0 ◂— 0xf000ff53
05:0014│      0x8fff4 —▸ 0x1005 —▸ 0x8955feeb —▸ 0 ◂— 0xf000ff53
06:0018│ ecx  0x8fff8 —▸ 0x7d76 —▸ 0x5300feeb —▸ 0 ◂— 0xf000ff53
07:001c│      0x8fffc —▸ 0x7d4e —▸ 0x7dbebb66 —▸ 0 ◂— 0xf000ff53
```

```assembly
 ► 0x11a4 <isr2+1>               push   0 ; push dummy error
   0x11a6 <isr2+3>               push   2 ; push int_no
   0x11a8 <isr2+5>               jmp    isr_common_stub <0x1170>

00:0000│ esp  0x8ffd4 —▸ 0x12bc (main+29) —▸ 0x839004cd —▸ 0 ◂— 0xf000ff53 ; eip
01:0004│      0x8ffd8 —▸ 8 —▸ 0xf000e2c3 —▸ 0 ◂— 0xf000ff53	 ; cs
02:0008│      0x8ffdc —▸ 2 —▸ 0xff53f000 —▸ 0 ◂— 0xf000ff53	 ; eflags
03:000c│      0x8ffe0 —▸ 0x90000 —▸ 0 —▸ 0xf000ff53 ◂— 0
04:0010│      0x8ffe4 —▸ 0x8fff8 —▸ 0x7d76 —▸ 0x5300feeb ◂— 0
05:0014│ ebp  0x8ffe8 —▸ 0x90000 —▸ 0 —▸ 0xf000ff53 ◂— 0
```

但是下面几项 `useresp, ss` 虽然在`registers_t`的结构体里面，但从调试来看，不是`push`进去的。



```c
 ► 0x1536 <isr_handler>       push   ebp
 
pwndbg> print(content)
$3 = {
  ds = 16, 
  edi = 0, 
  esi = 0, 
  ebp = 589800, 
  esp = 589772, 
  ebx = 32153, 
  edx = 0, 
  ecx = 589816, 
  eax = 14464, 
  int_no = 2, 
  err_code = 0, 
  eip = 4796, 
  cs = 8, 
  eflags = 2, 
  useresp = 589824,  => 0x90000
  ss = 589816		 => 0x8fff8
}
```

```assembly
 ► 0x1194 <isr_common_stub+36>    iretd
 
00:0000│ esp  0x8ffd4 —▸ 0x12bc (main+29) —▸ 0x839004cd —▸ 0 ◂— 0xf000ff53
01:0004│      0x8ffd8 —▸ 8 —▸ 0xf000e2c3 —▸ 0 ◂— 0xf000ff53
02:0008│      0x8ffdc —▸ 2 —▸ 0xff53f000 —▸ 0 ◂— 0xf000ff53
03:000c│      0x8ffe0 —▸ 0x90000 —▸ 0 —▸ 0xf000ff53 ◂— 0
04:0010│      0x8ffe4 —▸ 0x8fff8 —▸ 0x7d76 —▸ 0x5300feeb ◂— 0
05:0014│ ebp  0x8ffe8 —▸ 0x90000 —▸ 0 —▸ 0xf000ff53 ◂— 0
```

```assembly
   0x12ba <main+27>    int    2
 ► 0x12bc <main+29>    int    4
 
00:0000│ esp  0x8ffe0 —▸ 0x90000 —▸ 0 —▸ 0xf000ff53 ◂— 0
01:0004│      0x8ffe4 —▸ 0x8fff8 —▸ 0x7d76 —▸ 0x5300feeb ◂— 0
02:0008│ ebp  0x8ffe8 —▸ 0x90000 —▸ 0 —▸ 0xf000ff53 ◂— 0
```

并未将`useresp, ss`给`pop`出去，所以暂且存疑



## 问题

仍出现了加载到内存中的错误，是内存大小设置不够大的缘故，最后将大小设置为`dh`寄存器的最大值解决了该问题

```asm
mov bx, KERNEL_OFFSET ; load kernel into 0x1000 
mov dh, 31 
mov dl, [BOOT_DRIVE] 
call disk_load
```



## gcc 

`gcc version 10` 的时候存在一些 `extern` 变量的问题，具体可以看 

```
https://gcc.gnu.org/gcc-10/porting_to.html
```

然后根据以下步骤，修改 `Makefile` 即可

```
https://stackoverflow.com/questions/5188267/checking-the-gcc-version-in-a-makefile
```

