; -------------------- putScaledSpriteClip --------------------
; Draws a scaled sprite to the graph buffer, with clipping.
;
; Version: 1.1
; Author: Badja <http://move.to/badja> <badja@alphalink.com.au>
; Date: 21 December 1999
;
; Modified by Eric Piel : LD and smaller and quicker < Eric.Piel@etu.utc.fr >
; Date: 28 May 2000
; Size: 208 bytes (213 bytes if XORing the sprite, 205 bytes if just Loading it)
;
; Input:
;   HL -> sprite
;    D = x-coordinate
;    E = y-coordinate
;    B = height of unscaled sprite
;    C = width of unscaled sprite (in bytes, so divide by 8)
;    A = scale factor ($01 to $00 <=> 0.4% to 100%)
;        eg: $80 is 50%
;
; Output:
;   The sprite is ORed to the graph buffer. To XOR the sprite
;   instead, add the following line near the top of your program:
;   #define _SSC_XOR
;   Any parts of the sprite that are off-screen will be clipped.
;   To just load it insert:
;   #define _SSC_LD
;
; Destroys:
;   AF, BC, DE, HL;


putScaledSpriteClip:
	push	hl
	ld	(_SSC_SetScale1),a
	ld	(_SSC_SetScale2),a
	ld	a,d
	ld	(_SSC_SetInitXPos),a

	ld	a,e
	ld	(_SSC_YPos),a
	ld	a,d

	ld	d,0
	bit	7,e			;is e <0?
	jr	z,_SSC_YIsPos
	dec	d			;then de <0
_SSC_YIsPos:
	ld	h,d
	ld	l,e
	add	hl,de
	add	hl,de
	add	hl,hl
	add	hl,hl
	ld	de,GRAPH_MEM      ;plotscreen
	add	hl,de
	ld	d,0
	bit	7,a
	jr	z,_SSC_XIsPos
	dec	d
_SSC_XIsPos:
	ld	e,a
	sra	e
	sra	e
	sra	e
	add	hl,de
	and	%00000111
	ld	(_SSC_SetPreShift),a
	neg
	add	a,8
	ld	(_SSC_SetBitsLeft),a
	ex	de,hl

	ld	a,c
	ld	(_SSC_SetByteWidth),a
	sla	a
	sla	a
	sla	a
	ld	(_SSC_SetPixelWidth),a
	pop	hl

	ld	c,0
	push	bc
	jr	_SSC_DoRow

_SSC_SpriteLoop:
_SSC_SetScale1 = $ + 1
	ld	a,0      ; scale factor (self-modified)
	add	a,c
	ld	c,a
	push	bc
	jr	z,_SSC_DoRow
	jr	c,_SSC_DoRow

_SSC_SetByteWidth = $ + 1
	ld	bc,$0000	; C = byte width of sprite data (self-modified)
	add	hl,bc
	jr	_SSC_SkipRow
_SSC_DoRow:
	push	de

_SSC_SetInitXPos = $ + 1
	ld	a,$00		; x-position of start of row (self-modified)
	ld	(_SSC_XPos),a

_SSC_SetPreShift = $ + 2
	ld	bc,$0000	; B = # bits before start of row (self-modified)
	ld	a,b
	or	a
	jr	z,_SSC_PutBitsLeft
	ld	a,(de)
_SSC_PreShift:
	rlca
	djnz	_SSC_PreShift
	ld	(de),a
_SSC_PutBitsLeft:
_SSC_SetBitsLeft = $ + 1
	ld	b,$00		; # bits to copy into first byte (self-modified)
_SSC_SetPixelWidth = $ + 1
	ld	a,$00		; pixel width of sprite data (self-modified)
	push	af
	jr	_SSC_DoPixel
_SSC_RowLoop:
	and	%00000111
	jr	nz,_SSC_SameByte
	inc	hl
_SSC_SameByte:
_SSC_SetScale2 = $ + 1
	ld	a,0      ; scale factor (self-modified)
	add	a,c
	ld	c,a
	jr	z,_SSC_DoPixel
	jr	nc,_SSC_SkipPixel
_SSC_DoPixel:
_SSC_XPos	= $ + 1
	ld	a,00		; current x-position (self-modified)
	inc	a
	ld	(_SSC_XPos),a
	dec	a
	cp	96
	jr	nc,_SSC_OffScreen
_SSC_YPos	= $ + 1
	ld	a,00		; current y-position (self-modified)
	cp	64
	jr	nc,_SSC_OffScreen

	ld	a,(de)
	rlc	(hl)
#ifndef _SSC_LD
	jr	nc,_SSC_LeaveBit
#ifdef _SSC_XOR
	bit	7,a
	jr	z,_SSC_LeaveCarry
	ccf
_SSC_LeaveCarry:
#endif
#endif
	rla
	jr	_SSC_DoneBit

_SSC_OffScreen:
	ld	a,(de)
	rlc	(hl)
_SSC_LeaveBit:
	rlca
_SSC_DoneBit:
	ld	(de),a

	djnz	_SSC_DoneByte
	ld	b,8
	inc	de
	jr	_SSC_DoneByte
_SSC_SkipPixel:
	rlc	(hl)
_SSC_DoneByte:
	pop	af
	dec	a
	push	af
	jr	nz,_SSC_RowLoop
_SSC_DoneRow:
	inc	hl
	bit	3,b
	jr	nz,_SSC_RowComplete
	ld	a,(de)
_SSC_PostShift:
	rlca
	djnz	_SSC_PostShift
	ld	(de),a
	inc	de
_SSC_RowComplete:
	pop	af
	ex	(sp),hl		;pop	de\ push hl
	ld	de,12
	add	hl,de
	ex	de,hl
	pop	hl

	ld	a,(_SSC_YPos)
	inc	a
	ld	(_SSC_YPos),a

_SSC_SkipRow:
	pop	bc
	dec	b
	jp	nz,_SSC_SpriteLoop
	ret