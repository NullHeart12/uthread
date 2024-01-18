.code
jump_fcontext PROC FRAME
	.endprolog

	lea rsp, [rsp - 118h]

	movaps [rsp], xmm6
	movaps [rsp+10h], xmm7
	movaps [rsp+20h], xmm8
	movaps [rsp+30h], xmm9
	movaps [rsp+40h], xmm10
	movaps [rsp+50h], xmm11
	movaps [rsp+60h], xmm12
	movaps [rsp+70h], xmm13
	movaps [rsp+80h], xmm14
	movaps [rsp+90h], xmm15

	stmxcsr [rsp+0a0h]
	fnstcw [rsp+0a4h]

	mov r10, gs:[030h]
	mov rax, [r10 + 020h]
	mov [rsp + 0b0h], rax
	mov rax, [r10 + 01478h]
	mov [rsp + 0b8h], rax
	mov rax, [r10 + 010h]
	mov [rsp + 0c0h], rax
	mov rax, [r10 + 008h]
	mov [rsp + 0c8h], rax

	mov [rsp + 0d0h], r12
	mov [rsp + 0d8h], r13
	mov [rsp + 0e0h], r14
	mov [rsp + 0e8h], r15
	mov [rsp + 0f0h], rdi
	mov [rsp + 0f8h], rsi
	mov [rsp + 100h], rbx
	mov [rsp + 108h], rbp

	mov [rsp + 110h], rcx

	mov r9, rsp
	mov rsp, rdx

	movaps xmm6, [rsp]
	movaps xmm7, [rsp+10h]
	movaps xmm8, [rsp+20h]
	movaps xmm9, [rsp+30h]
	movaps xmm10, [rsp+40h]
	movaps xmm11, [rsp+50h]
	movaps xmm12, [rsp+60h]
	movaps xmm13, [rsp+70h]
	movaps xmm14, [rsp+80h]
	movaps xmm15, [rsp+90h]

	ldmxcsr [rsp+0a0h]
	fldcw [rsp+0a4h]

	mov r10, gs:[030h]
	mov rax, [rsp + 0b0h]
	mov [r10 + 020h], rax
	mov rax, [rsp + 0b8h]
	mov [r10 + 01478h], rax
	mov rax, [rsp + 0c0h]
	mov [r10 + 010h], rax
	mov rax, [rsp + 0c8h]
	mov [r10 + 008h], rax

	mov r12, [rsp + 0d0h]
	mov r13, [rsp + 0d8h]
	mov r14, [rsp + 0e0h]
	mov r15, [rsp + 0e8h]
	mov rdi, [rsp + 0f0h]
	mov rsi, [rsp + 0f8h]
	mov rbx, [rsp + 100h]
	mov rbp, [rsp + 108h]

	mov rax, [rsp + 110h]

	lea rsp, [rsp + 118h]

	pop r10

	mov [rax], r9
	mov [rax + 08h], r8

	mov rcx, rax

	jmp r10

jump_fcontext ENDP
END
