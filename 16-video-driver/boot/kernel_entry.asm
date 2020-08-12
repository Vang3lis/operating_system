[bits 32]
[extern main] ; Define calling point. Must have same name as kernel.c 'main' function
call main ; Call the C function
jmp $
