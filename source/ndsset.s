@//
@//  Fast and alignment-safe memset routine(s)
@//
@//! \file fg_ndsset.s
@//! \author J Vijn
@//! \date 20080701 - 20080719
@/* === NOTES ===
@* The off-alignment cases are optimized for size, not speed.
@* I'm fairly certain the corrections for size oversteps 
@can be removed somehow.
@*/


@ ---------------------------------------------------------------------
@ void ndsset  (u8  src, void *dst, uint size);
@ void ndsset16(u16 src, void *dst, uint size);
@ void ndsset32(u32 src, void *dst, uint size);
@ ---------------------------------------------------------------------

	.global ndsset
	.global ndsset16
	.global ndsset32

	@ .section .itcm, "ax", %progbits
	.text
	.arm
	.align

	@ *** 8-bit entry point ***
ndsset:
	orr		r0, r0, lsl #8
	@ *** 16-bit entry point ***
ndsset16:
	orr		r0, r0, lsl #16
	@ *** 32-bit entry point ***
ndsset32:
	cmp		r2, #0
	bxeq	lr
	ands	r3, r1, #3
	bne		.Lndsset_head
	
.Lndsset_main:
	@ dst should be aligned at this point!
	cmp		r2, #32					@ Jump over multi-word block
	blo		.Lndsset_word_loop

	@ --- Block run ---
	stmfd	sp!, {r4-r8}
	mov		r3, r0
	mov		r4, r0
	mov		r5, r0
	mov		r6, r0
	mov		r7, r0
	mov		r8, r0
	mov		ip, r0
.Lndsset_block_loop:
		subs	r2, r2, #32
		stmhsia	r1!, {r0, r3-r8, ip}
		bhi		.Lndsset_block_loop
	ldmfd	sp!, {r4-r8}
	bxeq	lr
	addne	r2, r2, #32			@ Correct for overstep in loop (redundant?)

	@ --- Word run ---
.Lndsset_word_loop:
		subs	r2, r2, #4
		strhs	r0, [r1], #4
		bhi		.Lndsset_word_loop
	bxeq	lr
	addne	r2, r2, #4			@ Correct for overstep (size=1,2,3)

	@ --- Unaligned tail ---
.Lndsset_tail:
	mov		r2, r2, lsl #3
	mov		ip, #1					@ - create mask
	rsb		r3, ip, ip, lsl r2		@ /
	b		.Lndsset_insert
	
	@ --- Unaligned head ---
.Lndsset_head:
	@ r2 : size
	@ r3 : left / insert-mask
	@ ip : free

	bic		r1, r1, #3				@ align dst

	add		r2, r3, r2
	movs	r3, r3, lsl #3			@ left*8 ; clear carry
	mov		ip, #1
	sbc		r3, ip, ip, lsl r3		@ -(1<<8*left) +1-1
	subs	r2, r2, #4				@ size= right-4

	@ If negative, this is a single-word fill
	@ This requires a truncated mask (like 0x0000FF00)
	sbclo	r2, ip, r2, lsl #3		@ x= -8*size +1-1
	andlo	r3, r3, r3, lsr r2		@ mask= mask & (mask>>-8*size);

	@ This is used by both head and tail
.Lndsset_insert:
	ldr		ip, [r1]
	bic		ip, ip, r3				@ Yeah, yeah. Interlock.
	and		r3, r0, r3
	orr		ip, ip, r3
	str		ip, [r1], #4

	bhi		.Lndsset_main			@ Longer stretch : go back.
	bx		lr						@ Single-word fill : finished.

	.size ndsset, .-ndsset

@ EOF
