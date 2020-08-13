print:
	push ax
	jmp start 

start:
	mov al, [bx]
	cmp al, 0
	je done
	
	mov ah, 0x0e
	int 0x10

	add bx, 1
	jmp start 

done:
	pop ax
	ret

print_nl:
	push ax
	
	mov ah, 0x0e
	mov al, 0x0a
	int 0x10

	mov al, 0x0d
	int 0x10

	pop ax
	ret
