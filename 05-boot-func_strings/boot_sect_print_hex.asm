[org 0x7c00]

print_hex:
	push eax
	push ebx
	push ecx
	push edx 
	jmp loop


loop:
	; ebx = addr; ecx = lenth
	cmp ecx, 0
	je finish

	sub ecx, 1
	push ecx ; 49 

	mov edx, [ebx]
	shl edx, 24
	shr edx, 24
	; store in the stack: ori hex 
	push edx ; 36
	mov ecx, mystring
	shr edx, 4
	; store in the stack: high 4 bits
	push edx ; 35
	add ecx, edx 
	mov al, [ecx]
	mov ah, 0x0e
	int 0x10
	
	; eax: high 4 bits 
	; edx: ori hex 
	pop eax ; 27
	pop edx ; 23
	mov ecx, mystring
	shl eax, 4
	sub edx, eax 
	add ecx, edx 
	mov al, [ecx]
	mov ah, 0x0e
	int 0x10

	mov al, [blank]
	mov ah, 0x0e
	int 0x10 
	
	pop ecx ; 17
	add ebx, 1
	jmp loop
	

finish:
	pop edx
	pop ecx
	pop ebx
	pop eax
	ret

mystring:
	db '0123456789abcdef', 0

blank:
	db ' ', 0
