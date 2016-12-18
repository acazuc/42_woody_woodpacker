; Le commentaire de la chatte a ta mere

section .text
	; Saving registers
	push rax
	push rbx
	push rcx
	push rdx
	push rdi
	push rsi
	pushf

	; Printing WOODY
	mov rax, 1
	mov rdi, rax
	lea rsi, [rel woody]
	mov rdx, 14
	syscall

	; Loading start & end addresses
	lea rax, [rel head]
	lea rbx, [rel tail]
	mov cl, 0x94

	; Decrypting
lstart: mov al, [rax]
	xor [rax], cl
	mov cl, dl
	inc rax
	cmp rax, rbx
	jne lstart

	; Restoring registers
	popf
	pop rsi
	pop rdi
	pop rdx
	pop rcx
	pop rbx
	pop rax

	; Starting main program. Jump address is defined by packer
	jmp 0x00

	; Data part
head:	dq 0
tail:	dq 0
woody:	db '....WOODY....', 10
