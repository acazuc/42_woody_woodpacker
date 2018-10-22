; Le commentaire de la chatte a ta mere

section .text
	; Saving registers
top:	pushf
	push rax
	push rbx
	push rcx
	push rdx
	push rdi
	push rsi

	; Printing WOODY
	mov rax, 1
	mov rdi, rax
	lea rsi, [rel woody]
	mov rdx, 14
	syscall

	; Loading start & end addresses
	lea rax, [rel top]
	add rax, [rel hdoff]
	mov rbx, rax
	add rbx, [rel len]
	mov cl, 0x94	

	; Decrypting
lstart: mov dl, [rax]
	xor [rax], cl
	mov cl, dl
	inc rax
	cmp rax, rbx
	jne lstart

	; Restoring registers
	pop rsi
	pop rdi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	popf

	; Starting main program. Jump address is defined by packer
	jmp 0x00

	; Data part. Head offset and lenght also defined by packer
hdoff:	dq 0
len:	dq 0
woody:	db '....WOODY....', 10
