
; ds: data seg
; address = seg << 4 + address
mov bx, 0x7c0
mov ds, bx
mov al, [the_flag]
mov ah, 0x0e
int 0x10

; but cs (code) es ss are 0
mov bx, 0x7c0
mov es, bx
mov al, [es:the_flag]
int 0x10

jmp $

the_flag:
	db "a", 0

times 510-($-$$) db 0
db 0x55, 0xaa


