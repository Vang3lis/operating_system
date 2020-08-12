# README

## 需求

对于`MAX_ROWS MAX_COLS`进行定义，最终实现供外部调用的`clear_screen() | kprint_at(char* message, int col, int row) | kprint(char* message)`

```
screen.h

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

void clear_screen();
void kprint_at(char* message, int col, int row);
void kprint(char* message);
```

实现 `screen.c` 中 `private functions: print_char() | get_cursor_offset() | set_cursor_offset() | get_offset() | get_offset_row | get_offset_col`

### print_char

```
/*
 * Innermost print function for our kernel, directly accesses the video memory
 *
 * if 'col' and 'row' are negative, we will print at current cursor location
 * if 'attr' is zero it will use 'white on black' as default
 * if (col, row) is not satisfied, then print 'E' on the lastest vidmem and attr==0xf4
 * if c=='\n', then col=0 row+=1
 * returns the offset of the next character
 * sets the video cursor to the returned offset 
 * */
 int print_char(char c, int col, int row, char attr)
```

如果 `col` 和`row` 为负数，则在当前光标处打印

如果`attr`未设置，则默认为`0x0f`

如果`(col, row) == (row*MAX_COLS + col) * 2`设置为不满足即越界的，则在最后打印 `E` `attr==0xf4`

如果 `c=='\n'`，则设置 `col=0 row+=1`



### clear_screen()

需要清空屏幕，并且将光标设置到`(0,0)`



## 问题

在程序编写过程中，没什么难点，因为经过了编号 15 文件夹的编写，但是在`make run`时候发现出错了，并且最开始定位错误的时候发现出错位置为`0x13fe port_byte_out()。`（一个比较奇怪的位置，这个位置在编号15的文件中编译和运行并未出现问题

在用 `x/10bx 0x13fe` 的时候，发现除了最初的两个字节是正确的，后面的均为`\x00` ，但是对于`kernel.elf`和`kernel.bin`进行反汇编和反编译的时候均没出现问题，于是陷入了僵局。

最开始对于`Makefile`进行查看，回想最后生成 `os-image`的地方为 `cat boot/bootsect.bin kernel.bin > os-image` ，在静态下`kernel.elf`和`kernel.bin`文件中`port_byte_out()`的地址都为`0x3fe`，而在静态下`os-image`文件中偏移为`0x5fe`，最初的想法是，是不是这个偏移错了，因此最后运行的时候，出现了加载不正确的问题。

后来觉得`kernel.bin`文件应该没出错，那么进而排查`bootsect.bin`文件的问题，突然想到，这个`0x13fe==0x1000+0x3fe`得到的，并且如何跳转到内核的，都是先加载，再`jmp 0x1000`，因此很明显问题就出现在加载进内存的时候，设置扇区数量太少的原因，然后进而我对于15文件的`os-image`和16文件的`os-image`分别查看大小，一个为1100+，一个为2000+，感觉应该就是这个问题，并且想到`0x13fe+2 == 0x1400`恰好是`0x100`的整数倍，就回去查看`dist_load`地方的编写，以及每一个单位的设置加载到内存的`byte`的大小是多少。

```
bootsect.asm
[bits 16]
load_kernel:
	mov ebx, MSG_LOAD_KERNEL
	call print  
	call print_nl 

	mov bx, KERNEL_OFFSET ; load kernel into 0x1000 
	mov dh, 2 ===> 16
	mov dl, [BOOT_DRIVE] 
	call disk_load
	ret
```

而一个扇区就为`512==0x200`恰好就是`0x200*2`，于是把加载扇区数调大就可以了。