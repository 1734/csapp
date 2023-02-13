	.file	"450-bufdemo.c"
	.text
	.globl	gets
	.type	gets, @function
gets:
.LFB39:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	%rdi, %rbp
	movq	%rdi, %rbx
	jmp	.L2
.L4:
	movb	%al, (%rbx)
	leaq	1(%rbx), %rbx
.L2:
	movq	stdin(%rip), %rdi
	call	getc@PLT
	cmpl	$10, %eax
	je	.L3
	cmpl	$-1, %eax
	jne	.L4
.L3:
	cmpl	$-1, %eax
	sete	%dl
	cmpq	%rbp, %rbx
	sete	%al
	testb	%al, %dl
	jne	.L6
	movb	$0, (%rbx)
	movq	%rbp, %rax
.L1:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L6:
	.cfi_restore_state
	movl	$0, %eax
	jmp	.L1
	.cfi_endproc
.LFE39:
	.size	gets, .-gets
	.globl	echo
	.type	echo, @function
echo:
.LFB40:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	leaq	8(%rsp), %rbx
	movq	%rbx, %rdi
	call	gets
	movq	%rbx, %rdi
	call	puts@PLT
	addq	$16, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE40:
	.size	echo, .-echo
	.globl	good_echo
	.type	good_echo, @function
good_echo:
.LFB41:
	.cfi_startproc
	endbr64
	subq	$24, %rsp
	.cfi_def_cfa_offset 32
	leaq	8(%rsp), %rdi
	movq	stdin(%rip), %rdx
	movl	$8, %esi
	call	fgets@PLT
	testq	%rax, %rax
	je	.L10
	movq	%rax, %rdi
	call	puts@PLT
.L10:
	addq	$24, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE41:
	.size	good_echo, .-good_echo
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Type a string:"
	.text
	.globl	main
	.type	main, @function
main:
.LFB42:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movl	%edi, %ebx
	leaq	.LC0(%rip), %rsi
	movl	$1, %edi
	movl	$0, %eax
	call	__printf_chk@PLT
	cmpl	$1, %ebx
	jle	.L14
	movl	$0, %eax
	call	good_echo
.L15:
	movl	$0, %eax
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	ret
.L14:
	.cfi_restore_state
	movl	$0, %eax
	call	echo
	jmp	.L15
	.cfi_endproc
.LFE42:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
