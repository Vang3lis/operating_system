disk_load:
	push dx

	mov ah, 0x02 
	mov al, dh   ; the num of READ sectors
	mov ch, 0x00 ; select cylinder 0
	mov dh, 0x00 ; select head 0
	mov cl, 0x02 ; the second sector

	int 0x13
	
	jc disk_error ; if error, carry flag set

	pop dx
	cmp dh, al
	jne disk_error

	ret 

disk_error:
	mov bx, DISK_ERROR
	call print

DISK_ERROR:
	db "Disk read error", 0

