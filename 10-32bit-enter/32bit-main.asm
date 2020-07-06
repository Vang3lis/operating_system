[org 0x7c00]
	mov bp, 0x9000
	mov sp, bp

	mov bx, MSG_REAL_MODE
	call print

	call switch_to_pm
	jmp $

%include "../05-boot-func_strings/boot_sect_print.asm"
%include "../09-32bit-gdt/32bit-gdt.asm"
%include "../08-32bit-print/32bit-print.asm"
%include "./32bit-swtich.asm"

[bits 32]
BEGIN_PM:
	mov bx, MSG_PROT_MODE
	call print_string_pm
	jmp $

MSG_REAL_MODE db "Started in 16-bit real mode", 0
MSG_PROT_MODE db "Loaded 32-bit protected mode", 0

times 510-($-$$) db 0
dw 0xaa55
