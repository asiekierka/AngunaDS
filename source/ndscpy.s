@
@
@
/*
	ndscpy.s created by Cearn.
	http://www.coranac.com/


=== NOTES ===
  * The unaligned heads (especially when combined with small sizes)
    are horrible no matter what you do. Screw them. and focus 
	optimizing elsewhere.
*/


@ ---------------------------------------------------------------------
@ EXTERN_C void ndscpy(const void *srcv, void *dstv, uint size);
@ ---------------------------------------------------------------------

	@ .section .iwram, "ax", %progbits
	.text
	.arm
	.balign 32
	.global ndscpy
	.type ndscpy, %function
ndscpy:
	cmp		r2, #0
	bxeq	lr
	stmfd	sp!, {r4-r9}

	@ dst-alignment branches
	movs		r3, r1, lsl #31	@ Test dst alignment
	bmi			.Lndscpy_dst_a0	@ dst is byte-aligned
	bcs			.Lndscpy_dst_a1	@ dst is hword-aligned

.Lndscpy_main:
	ands		r3,	r0, #3		@ Test src alignment
	bne			.Lndscpy_ne		@ Source not aligned :(

	@ --- Main, aligned dst and src ---
.Lndscpy_eq:
	
	@ Copy in 32-byte chunks
.Lndscpy_eq_blocks:
		subs	r2, r2, #32
		ldmhsia	r0!, {r3-r9, ip}
		stmhsia	r1!, {r3-r9, ip}
		bhi		.Lndscpy_eq_blocks
	@ Early return
	ldmeqfd	sp!, {r4-r9}
	bxeq	lr
	
	@ --- Tail (31 byte max) ---
	movs	ip, r2, lsl #28		@ test bits 4/3
	ldmcsia	r0!, {r4-r7}
	stmcsia	r1!, {r4-r7}
	ldmmiia	r0!, {r4-r5}
	stmmiia	r1!, {r4-r5}

	movs	ip, r2, lsl #30		@ test bits 2/1
	ldmcsia	r0!, {r3}
	ldrmih	r4, [r0], #2
	stmcsia	r1!, {r3}
	strmih	r4, [r1], #2

.Lndscpy_last_byte:
	tst		r2, #1				@ last byte
	ldrneb	r3, [r0]
	ldrneb	ip, [r1, #1]
	orrne	r3, r3, ip, lsl #8
	strneh	r3, [r1]

.Lndscpy_return:
	ldmfd	sp!, {r4-r9}
	bx		lr

	@ --- Main, aligned dst, misaligned src ---
	@ NOTE: this is still pretty unoptimized
.Lndscpy_ne:
	bic		r0, r0, #3
	mov		r9, r3, lsl #3		@ rshift
	rsb		r8, r9, #32			@ lshift

	subs	r2, r2, #16
	blo		.Lndscpy_ne_blocks_end

	ldr		r3, [r0]
	mov		r3, r3, lsr r9

	@ In 16-byte chunks
	@ src |    |defg|hijk|lmno|pxxx|
	@ r3: |xabc| r4 | r5 | r6 | r7 |
	@ tmp |abcd|efgh|ijkl|mnop|    |xxx0| -> r3
	@ dst | r3 | r4 | r5 | r6 |

	@# NOTE: reg-shifts cost 1I extra :(
	@# Splitting into #8 / #16 / #24 would be faster
	
	@# PONDER: what about cmn?

.Lndscpy_ne_blocks:
		ldmib	r0!, {r4-r7}
		orr		r3, r3, r4, lsl r8
		mov		r4, r4, lsr r9
		orr		r4, r4, r5, lsl r8
		mov		r5, r5, lsr r9
		orr		r5, r5, r6, lsl r8
		mov		r6, r6, lsr r9
		orr		r6, r6, r7, lsl r8
		subs	r2, r2, #16
		stmia	r1!, {r3-r6}
		mov		r3, r7, lsr r9
		bhs		.Lndscpy_ne_blocks
	beq		.Lndscpy_return
.Lndscpy_ne_blocks_end:
	add		r2, r2, #16


	@ Tail in hwords
	add		r0, r0, r9, lsr #3		@ Un-align src again.
	subs	r2, r2, #2
	blo		.Lndscpy_last_byte

.Lndscpy_ne_bytes:
		ldrb	r3, [r0], #1
		ldrb	r4, [r0], #1
		subs	r2, r2, #2
		orr		r3, r3, r4, lsl #8
		strh	r3, [r1], #2
		bhs		.Lndscpy_ne_bytes
	b	.Lndscpy_last_byte

	@ --- Shared head ---

	@ Align to (half)word (assumes dst&1)
.Lndscpy_dst_a0:
	ldrb	r3, [r0], #1
	ldrb	ip, [r1, #-1]!
	subs	r2, r2, #1
	orr		ip, ip, r3, lsl #8
	strh	ip, [r1], #2
	bls		.Lndscpy_return		@ Single-byte: quit

	tst		r1, #2
	beq		.Lndscpy_main		@ If word-aligned: go back

	@ Align to word (assumes dst&2)
.Lndscpy_dst_a1:
	ldrb	ip, [r0], #1
	subs	r2, r2, #1
	ldrhib	r3, [r0], #1		@ More bytes: take next from src
	ldrlsb	r3, [r1]			@ Single byte: take from dst
	subs	r2, r2, #1	
	orr		ip, ip, r3, lsl #8
	strh	ip, [r1], #2
	bhi		.Lndscpy_main
	b		.Lndscpy_return		@ Two bytes: quit
	
	.size	ndscpy, .-ndscpy

@ EOF
