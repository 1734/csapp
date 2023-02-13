	.file	"480-charbuf.c"
	.text
	.globl	len
	.type	len, @function
len:
.LFB51:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	call	strlen@PLT
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE51:
	.size	len, .-len
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%ld"
	.text
	.globl	iptoa
	.type	iptoa, @function
iptoa:
.LFB52:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movq	(%rsi), %r8
	leaq	.LC0(%rip), %rcx
	movq	$-1, %rdx
	movl	$1, %esi
	movl	$0, %eax
	call	__sprintf_chk@PLT
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE52:
	.size	iptoa, .-iptoa
	.globl	intlen
	.type	intlen, @function
intlen:
.LFB53:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$32, %rsp
	.cfi_def_cfa_offset 48
	movq	%rdi, 24(%rsp)
	leaq	24(%rsp), %rsi
	leaq	12(%rsp), %rbx
	movq	%rbx, %rdi
	call	iptoa
	movq	%rbx, %rdi
	call	len
	addq	$32, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE53:
	.size	intlen, .-intlen
	.section	.rodata.str1.1
.LC1:
	.string	"%d"
	.text
	.globl	itoa
	.type	itoa, @function
itoa:
.LFB54:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	%esi, %r8d
	leaq	.LC1(%rip), %rcx
	movq	$-1, %rdx
	movl	$1, %esi
	movl	$0, %eax
	call	__sprintf_chk@PLT
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE54:
	.size	itoa, .-itoa
	.globl	storeint
	.type	storeint, @function
storeint:
.LFB55:
	.cfi_startproc
	endbr64
	movl	%edi, (%rsi)
	ret
	.cfi_endproc
.LFE55:
	.size	storeint, .-storeint
	.globl	intlen2
	.type	intlen2, @function
intlen2:
.LFB56:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	leaq	12(%rsp), %rsi
	call	storeint
	movq	%rsp, %rbx
	movl	12(%rsp), %esi
	movq	%rbx, %rdi
	call	itoa
	movq	%rbx, %rdi
	call	len
	addq	$16, %rsp
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE56:
	.size	intlen2, .-intlen2
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
