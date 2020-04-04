**int 0x13**
当设置`ah=0x2` 且调用`int 0x13`时就是从磁盘中读取数据到内存

先提出几个名词 柱面`cylinder` 磁道`track`磁头`head`扇区`sector` （1磁道=16扇区，1扇区=512字节）

`boot sector`是第一个扇区

> <https://stanislavs.org/helppc/int_13-2.html>

```
AH = 02
AL = number of sectors to read	(1-128 dec.)
CH = track/cylinder number  (0-1023 dec., see below)
CL = sector number  (1-17 dec.)
DH = head number  (0-15 dec.)
DL = drive number (0=A:, 1=2nd floppy, 80h=drive 0, 81h=drive 1)
ES:BX = pointer to buffer


on return:
AH = status  (see INT 13,STATUS)
AL = number of sectors read
CF = 0 if successful
= 1 if error
```

`al` 是读取数目，`ch` 是`cylinder`编号（从0开始），`cl`是扇区编号（从1开始），`dh`是磁头编号（从0开始），`es:bx`是被写到内存的地址

如果`error`则`carry flag`会被设为1

最后运行的时候用 `qemu-system-x86_64 -fda ./bin`，这样会设置`dl=0x0`

`-fda/-fdb file  use 'file' as floppy disk 0/1 image`

其中其他人的地方提到 如果 error的话可以尝试`qemu ./bin -boot c`，该情况会设置`dl=0x80`

> <https://www.cs.bham.ac.uk/~exr/lectures/opsys/10_11/lectures/os-dev.pdf> : 26 | 30/77