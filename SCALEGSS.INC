; -------------------- getScaledSize --------------------
; Calculates the scaled size of a sprite.
;
; Version: 1.1
; Author: Badja <http://move.to/badja> <badja@alphalink.com.au>
; Date: 21 December 1999
; Size: 19 bytes
;
; Modified by Eric Piel : quicker and registers fit the 1.1 of scale routine < Eric.Piel@etu.utc.fr >
; Date: 28 May 2000
; Size: 27 bytes
;
; Input:
;    B = height of unscaled sprite
;    C = width of unscaled sprite (in pixels)
;    A = scale factor ($01 to $00 <=> 0.4% to 100%)
;        eg: $80 is 50%
;
; Output:
;    A = height of scaled sprite
;    H = width of scaled sprite
;
; Destroys:
;    AF, B, DE, HL



getScaledSize:
	or	a
	jr	nz,gss_Compute
	ld	a,b
	ld	h,c
	ret
gss_Compute:
	ld	l,a
	ld	h,b
	call	gss_MUL_HL_S
	ld	l,a
	ld	a,h
	ld	h,c
;	jp	gss_MUL_HL_S


;----------------------------------------------------------------------------
;[ MUL_HL_S ] by Hideaki Omuro
; optimized for size (13 bytes, 315-363 clocks)
;
;parameters:
; H = 8-bit multiplicand, L = 8-bit multiplier
;returns:
; HL = 16-bit product
;modifies:
; BDEFHL
;----------------------------------------------------------------------------
gss_MUL_HL_S:
   LD    D, 0                          ; DE = multiplier, H = multiplicand
   LD    E, L
   LD    L, D                          ; result = 0

   LD    B, 8                          ; 8 loops (8 bits)

_CMH_mulloop:
   ADD   HL, HL                        ; shift/test bit
   JR    NC, _CMH_noadd                ; if shifted bit is set then
   ADD   HL, DE                        ; add the multiplier
_CMH_noadd:
   DJNZ  _CMH_mulloop

   RET 