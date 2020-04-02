[org 0x7c00]

mov bx, Hello
call print

call print_nl

mov bx, GoodBye
call print

call print_nl

mov ebx, Hello
mov ecx, 12
call print_hex

call print_nl

mov ebx, GoodBye
mov ecx, 7
call print_hex

call print_nl

%include "boot_sect_print.asm"
%include "boot_sect_print_hex.asm"


Hello:
	db 'Hello, World', 0

GoodBye:
	db 'GoodBye', 0

times 510-($-$$) db 0
db 0x55, 0xaa
