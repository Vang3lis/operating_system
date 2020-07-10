![](/home/vangelis/Github/operating_system/03-boot-sect/深度截图_选择区域_20200710222417.png)

Boot Sector 加载到内存中基址为 0x7c00



## 全局偏移地址

对引导程序来说，偏移地址 `0x7c00` 是会经常用到的，但是总是使用硬编码地址是非常不方便的。所以现代汇编器都提供了一个定义全局偏移地址的命令 `org` ：

```asm
[org 0x7c00]
```