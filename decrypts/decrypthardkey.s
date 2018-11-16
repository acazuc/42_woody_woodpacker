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
	push r8
	push r9

	; --- Get Key ---

	; Printing GIVE KEY PLS
	mov rax, 1
	mov rdi, rax
	lea rsi, [rel givekey]
	mov rdx, 12
	syscall

	; Clear rbx, for use as key register
	mov rbx, 0x0123456789ABCDEF
	mov r9, 0x071F3B5989B3C7EF

	; Read one char
getone:	mov rax, 0
	mov rdi, rax
	mov rsi, rsp
	mov rdx, 1
	syscall

	; Test for failure or empty read
	cmp rax, 0
	jl bad
	jz getone

	; Test for end of key string
	mov al, [rsp]
	cmp al, 10
	jz keydone

	; Prepare key set loop
	mov rdx, 8

	; Add curent char to key
ltop:	xor r8, r8
	mov r8b, r9b
	imul r8, rax
	add bl, r8b

	; Shift factor, shift key to next byte and jump if not end of loop
	ror r9, 12
	ror rbx, 8
	dec rdx
	jnz ltop

	; Rotate factor
	xor rcx, rcx
	mov cl, al
	imul rcx, 8
	ror r9, cl

	; Go to next char
	jmp getone

	; Exit in case of failed read
bad:	mov rax, 60
	mov rdi, 0
	syscall

	; --- Decrypt ---

	; Printing WOODY
keydone:mov rax, 1
	mov rdi, rax
	lea rsi, [rel woody]
	mov rdx, 14
	syscall

	; Loading start address, section length & key
	lea rax, [rel top]
	add rax, [rel hdoff]
	mov rcx, [rel len]

	; Decrypting
lstart: xor [rax], bl
	mov bl, [rax]
	ror rbx, 31
	inc rax
	dec rcx
	jnz lstart

	; Restoring registers
	pop r9
	pop r8
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
givekey:db 'Enter key : '
woody:	db '....WOODY....', 10
