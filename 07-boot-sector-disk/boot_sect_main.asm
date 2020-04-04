[org 0x7c00]

mov [BOOT_DRIVER], dl ; BIOS stores our boot driver in dl

mov bp, 0x8000
mov sp, bp

mov bx, 0x9000 ; load 5 sectors to 0x0000(ES):0x9000(BX) from the boot disk
mov dh, 5
mov dl, [BOOT_DRIVER]
call disk_load

mov bx, 0x9000
mov cx, 8
call print_hex

mov bx, 0x9400
mov cx, 8
call print_hex

jmp $

%include "../05-boot-func_strings/boot_sect_print.asm"
%include "../05-boot-func_strings/boot_sect_print_hex.asm"
%include "boot_sect_disk_load.asm"

BOOT_DRIVER: db 0
times 510-($-$$) db 0
dw 0xaa55

times 128 dq 0x12345678
times 128 dq 0xdeadbeef



