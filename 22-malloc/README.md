# README

## 需求

```
Goal: Implement a memory allocator
```

目标就是在`shell`中实现内存分配的指令

### kernel

实现对于`PAGE`指令的内核操作

### libc

在`mem.c`中实现`kmalloc`，具体接口如下

size 分配的大小，align是否对齐，phys_addr为修改物理地址的值，返回值为分配空间的虚拟地址

```c
u32 kmalloc(u32 size, int align, u32* phys_addr)
```

并且根据[03-boot-sect](https://github.com/Vang3lis/operating_system/tree/master/03-boot-sect)小节中内容，可知空余的空间从 `0x10000` 开始，则将其设为 `free_mem_addr`，此后分配的内存从 `0x10000` 开始

P.S. 并未实现 `kfree` 操作


## 问题

在实现的过程中，发现 `free_mem_addr` 的值存在在二进制文件中，但是运行起来的时候，发现这个地址在内存中为0

通过重新翻看 [03-boot-sect](https://github.com/Vang3lis/operating_system/tree/master/03-boot-sect) 和 [07-boot-sector-disk](https://github.com/Vang3lis/operating_system/tree/master/07-boot-sector-disk) 可以得知，我们在加载内容到内存时，并未加载足够多的内存（在设置`dl`的值为`31`时只能加载内存区域为`0-0x4000`，而`free_mem_addr`为`0x5080`），通过将`dl`的值设置更大，可以载入更多的内存，而不能更大的原因是`0x7c00`之前就被加载到了内存，因故怀疑`0x7000-`之后的内容无法通过`disk_load` 进行加载

```python3
>>> hex(0x36 * 0x200)
'0x6c00'
```
