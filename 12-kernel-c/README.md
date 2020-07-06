```shell
gcc -ffreestanding -c function.c -o function.o
```

`-ffreestangding`  : A `freestanding` environment is one in which the standard library may not exist, and program startup may not necessarily be at "main". The option `-ffreestanding` directs the compiler to *not* assume that standard functions have their usual definition.

标准库文件不存在，而且这个程序起始可以不需要 "main" 

> <https://stackoverflow.com/questions/17692428/what-is-ffreestanding-option-in-gcc>



```shell
objdump -M intel -d function.o
```



```shell
ld -o function.bin -Ttext 0x0 --oformat binary function.o
```

可无视 `warning`

`-o` : 最终输出

`-Ttext` : The `-Ttext` option puts the [.text](https://en.wikipedia.org/wiki/Code_segment) section of your program by the given address.

设置 .text 段起始点

> <https://stackoverflow.com/questions/34622557/what-is-ld-ttext-option-doing>

`--oformat` : output-format 

设置输出格式



```
ndisasm -b 64 function.bin
```

反编译