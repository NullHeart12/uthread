EXTERN  _exit:PROC

.code
make_fcontext PROC FRAME
	.endprolog

	mov rax, rcx				;get the pointer to the context

	and rax, -16				; align to 16 bytes

	sub rax, 0150h				; allocate space for the context

	mov [rax + 0100h], r8		; store the third para: function pointer
	mov [rax + 0c8h], rcx		; store the first para: pointer to the context

	neg rdx
	lea rcx, [rcx + rdx]
	mov [rax + 0c0h], rcx		; store the limits

	mov [rax + 0b8h], rcx		; store the deallocation pointer
	
	stmxcsr [rax + 0a0h]		; store the mxcsr
	fnstcw [rax + 0a4h]			; store the fpu control word

	;xor rcx, rcx
	;mov [rax + 0b0h], rcx		; store the rcx=0

	lea rcx, [rax + 0140h]
	mov [rax + 0110h], rcx		; store the transfer_t

	lea rcx, trampoline
	mov [rax + 0118h], rcx		; store the jumptofuc address

	lea rcx, finish
	mov [rax + 0108h], rcx		; store the finish address

	ret

trampoline:
	push rbp
	jmp rbx

finish:
	xor rcx, rcx
	call _exit

make_fcontext ENDP
END