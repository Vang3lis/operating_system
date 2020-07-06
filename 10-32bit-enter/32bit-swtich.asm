[bits 16]
; Switch to protected mode

switch_to_pm:
	cli			; 1. disable interrupts
	lgdt [gdt_descriptor] ; 2. load gdt

	mov eax, cr0	; set the first bit of cr0 (a control register)
	or eax, 0x1
	mov cr0, eax 

	jmp CODE_SEG:init_pm	; Make a far jmp and force the CPU to flush its cache of pre-fetched and real-mode decoded instructions

[bits 32]
; initialise registers and the stack once in PM

init_pm:
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	mov ebp, 0x90000
	mov esp, ebp

	call BEGIN_PM

