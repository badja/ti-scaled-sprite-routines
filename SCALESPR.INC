; -------------------- putScaledSprite --------------------
; Draws a scaled sprite to the graph buffer.
;
; Version: 1.1
; Author: Badja <http://move.to/badja> <badja@alphalink.com.au>
; Date: 21 December 1999
;
; Modified by Eric Piel : LD and smaller and quicker < Eric.Piel@etu.utc.fr >
; Date: 19 May 2000
; Size: 156 bytes (161 bytes if XORing the sprite, 151 bytes if just Loading it)
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
;   #define _SS_XOR
;   To just load it insert:
;   #define _SS_LD
;
; Destroys:
;   AF, BC, DE, HL


putScaledSprite:
	ld	(_SS_SetScale1),a
	ld	(_SS_SetScale2),a
	ld	a,d

	push	hl
	ld	d,0
	ld	h,d
	ld	l,e
	add	hl,de
	add	hl,de
	add	hl,hl
	add	hl,hl
	ld	de,GRAPH_MEM		;plotscreen
	add	hl,de
	ld	d,0
	ld	e,a
	srl	e
	srl	e
	srl	e
	add	hl,de
	and	%00000111
	ld	(_SS_SetPreShift),a
	neg
	add	a,8

	ld	(_SS_SetBitsLeft),a
	ex	de,hl

	ld	a,c
	ld	(_SS_SetByteWidth),a
	sla	a
	sla	a
	sla	a
	ld	(_SS_SetPixelWidth),a

	pop	hl
	ld	c,0
	push	bc
	jr	_SS_DoRow

_SS_SpriteLoop:
_SS_SetScale1 = $ + 1
	ld	a,0		; scale factor (self-modified)
	add	a,c
	ld	c,a
	push	bc
	jr	z,_SS_DoRow
	jr	c,_SS_DoRow
_SS_SetByteWidth = $ + 1
	ld	bc,$0000	; C = byte width of sprite data (self-modified)
	add	hl,bc
	jr	_SS_SkipRow
_SS_DoRow:
	push	de

_SS_SetPreShift = $ + 2
	ld	bc,$0000	; B = # bits before start of row (self-modified)
	ld	a,b
	or	a
	jr	z,_SS_PutBitsLeft
	ld	a,(de)
_SS_PreShift:
	rlca
	djnz	_SS_PreShift
	ld	(de),a
_SS_PutBitsLeft:
_SS_SetBitsLeft = $ + 1
	ld	b,$00		; # bits to copy into first byte (self-modified)

_SS_SetPixelWidth = $ + 1
	ld	a,$00		; pixel width of sprite data (self-modified)
	push	af
	jr	_SS_DoPixel
_SS_RowLoop:
	and	%00000111
	jr	nz,_SS_SameByte
	inc	hl
_SS_SameByte:
_SS_SetScale2 = $ + 1
	ld	a,0		; scale factor (self-modified)
	add	a,c
	ld	c,a
	jr	z,_SS_DoPixel
	jr	nc,_SS_SkipPixel
_SS_DoPixel:
	ld	a,(de)
	rlc	(hl)
#ifndef _SS_LD
	jr	nc,_SS_LeaveBit
#ifdef _SS_XOR
	bit	7,a
	jr	z,_SS_LeaveCarry
	ccf
_SS_LeaveCarry:
#endif
#endif
	rla
#ifndef _SS_LD
	jr	_SS_DoneBit
_SS_LeaveBit:
	rlca
_SS_DoneBit:
#endif
	ld	(de),a
	djnz	_SS_DoneByte
	ld	b,8
	inc	de
	jr	_SS_DoneByte
_SS_SkipPixel:
	rlc	(hl)
_SS_DoneByte:
	pop	af
	dec	a
	push	af
	jr	nz,_SS_RowLoop
_SS_DoneRow:
	inc	hl
	bit	3,b
	jr	nz,_SS_RowComplete
	ld	a,(de)
_SS_PostShift:
	rlca
	djnz	_SS_PostShift
	ld	(de),a
	inc	de
_SS_RowComplete:
	pop	af
	ex	(sp),hl
	ld	de,12
	add	hl,de
	ex	de,hl
	pop	hl

_SS_SkipRow:
	pop	bc
	djnz	_SS_SpriteLoop
	ret
