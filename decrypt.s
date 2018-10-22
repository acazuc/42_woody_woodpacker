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

	; *** Old Style ***

	; Loading start & end addresses
	;mov rax, [rel head]
	;mov rbx, [rel tail]
	;mov cl, 0x94

	; *** New Style ***

	; Loading start & end addresses
	lea rax, [rel top]
	add rax, [rel head]
	mov rbx, rax
	add rbx, [rel tail]
	mov cl, 0x94	

	; Decrypting
lstart: mov dl, [rax]
	xor [rax], cl
	mov cl, dl
	inc rax
	cmp rax, rbx
	jne lstart

	;mov rax, 1
	;mov rdi, rax
	;lea rsi, [rel woody]
	;mov rdx, 14
	;syscall

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

	; Data part
head:	dq 0
tail:	dq 0
woody:	db '....WOODY....', 10
