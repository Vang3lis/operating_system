# README

之前一直想自己写一个小型的操作系统，最后还是拖了很久，依葫芦画瓢写了大概一半的OS，以下就是已完成和未完成的模块

## Strategy
We will want to do many things with our OS:

+ Boot from scratch, without GRUB - DONE!
+ Enter 32-bit mode - DONE
+ Jump from Assembly to C - DONE!
+ Interrupt handling - DONE!
+ Screen output and keyboard input - DONE!
+ A tiny, basic libc which grows to suit our needs - DONE!
+ Memory management
+ Write a filesystem to store files
+ Create a very simple shell
+ User mode
+ Maybe we will write a simple text editor
+ Multiple processes and scheduling

Probably we will go through them in that order, however it's soon to tell.

If we feel brave enough:

+ A BASIC interpreter, like in the 70s!
+ A GUI
+ Networking

## 参考

> https://github.com/cfenollosa/os-tutorial

> https://github.com/ruiers/os-tutorial-cn