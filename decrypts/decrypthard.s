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

	; Loading start address, section length & key
	lea rax, [rel top]
	add rax, [rel hdoff]
	mov rbx, [rel len]
	mov rcx, [rel key]

	; Decrypting
lstart: xor [rax], cl
	mov cl, [rax]
	ror rcx, 31
	inc rax
	dec rbx
	jnz lstart

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

	; Data part. All zero fields are loaded by packer
hdoff:	dq 0
len:	dq 0
key:	dq 0
woody:	db '....WOODY....', 10
