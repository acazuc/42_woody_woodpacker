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

	; ----- KEY GET

	; Printing GIVE KEY PLS
	mov rax, 1
	mov rdi, rax
	lea rsi, [rel givekey]
	mov rdx, 12
	syscall

	; Clear rbx, for use as key register
	mov bl, 1
	mov rdx, 0x071F3B5989B3C7EF

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
	cmp rax, 10
	jz keydone

	; Prepare key set loop
	mov al, [rsp]
	xor r8, r8

	; Add curent char to key
	mov r8b, dl
	imul r8, rax
	add bl, r8b

	; Rotate factor
	xor rcx, rcx
	mov cl, al
	imul rcx, 8
	ror rdx, cl

	; Test for end of loop counter
	jmp getone

	; Exit in case of failed read
bad:	mov rax, 60
	mov rdi, 0
	syscall

keydone:
	mov cl, bl

	; END KEY GET

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
givekey:db 'Enter key : '
woody:	db '....WOODY....', 10
