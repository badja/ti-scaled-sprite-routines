; Sample Ion/CrASH program for using putScaledSprite v1.1 and getScaledSize v1.1
;
; Version: 1.1
; Author: Badja <http://badja.calc.org> <badja@calc.org>
; Date: 29 May 2000
; Ported to CrASH by Eric Piel

;#define TI82

#ifdef TI82

#include CRASH82.INC

#else

.NOLIST
#include "ion.inc"
.LIST

#endif


;#define _SS_XOR		; Comment this out to OR the sprite
#define _SS_LD			; Uncomment this out to LD the sprite

#define SPR_HEIGHT 50		; symbolic constants
#define SPR_WIDTH  88
#define X_OFFSET    4
#define Y_OFFSET    7

#ifndef TI82
#ifdef TI83P			; Ion header
	.org	progstart-2
	.db	$BB,$6D
#else
	.org	progstart
#endif
	ret
	jr	nc,start
#endif
	.db	"Scaled Sprite Test",0

start:
	ld	a,$04		; initial scale factor
	ld	b,64		; number of frames
aniLoop:
	push	bc
;#ifdef TI82
;	ld hl,GRAPH_MEM
;	ld (hl),0
;	ld de,GRAPH_MEM+1
;	ld bc,767
;	ldir
;#else
;	bcall(_cleargbuf)	; clear the graph buffer
;#endif

	push	af
	ld	b,SPR_HEIGHT
	ld	c,SPR_WIDTH
	call	getScaledSize	; A = scaled height, H = scaled width
	neg	
	add	a,SPR_HEIGHT+(2*Y_OFFSET)
	sra	a
	ld	e,a		; E = y-position

	ld	a,SPR_WIDTH+(2*X_OFFSET)
	sub	h
	sra	a
	ld	d,a		; D = x-position
	pop	af

	push	af
	ld	b,SPR_HEIGHT
	ld	c,SPR_WIDTH/8
	ld	hl,picShoes
	call	putScaledSprite
#ifdef TI82
	call	CR_GRBCopy
#else
	call	ionFastCopy
#endif
	pop	af

	add	a,$04		; increase scale factor

	pop	bc
	djnz	aniLoop		; display next frame

#ifdef TI82
	jp	CR_KHAND
#else
	bcall(_getkey)
	ret
#endif


#include "scalegss.inc"		; getScaledSize routine
#include "scalespr.inc"		; putScaledSprite routine

picShoes:
 .db %00000000,%00000000,%00000000,%00000000,%00000000,%01110000,%01111111,%11100000,%00000000,%00000000,%00000000
 .db %00000000,%00000000,%00000000,%00000000,%00000001,%10011111,%11111111,%11111111,%11000000,%00000000,%00000000
 .db %00000000,%00000000,%00000000,%00000000,%00000011,%11101111,%11111111,%11111111,%00100000,%00000000,%00000000
 .db %00000000,%00000000,%00000000,%00000000,%00000011,%11110111,%11111101,%01011111,%11100000,%00000000,%00000000
 .db %00000000,%00000000,%00000000,%00000000,%00000011,%11111111,%11101010,%10101111,%11100000,%00000000,%00000000
 .db %00000000,%00000000,%00000000,%00000000,%00000001,%11111111,%11010101,%01011111,%11111000,%00000000,%00000000
 .db %00000000,%00000000,%00000000,%00000000,%00000001,%11111111,%11111010,%10111100,%00000111,%10000000,%00000000
 .db %00000000,%00000000,%00000000,%00000000,%00000011,%10000000,%00000111,%11100001,%00111000,%01110000,%00000000
 .db %00000000,%00000000,%00000000,%00000111,%11011100,%00011111,%11111100,%00001111,%11111111,%10001000,%00000000
 .db %00000000,%00000000,%00000000,%00011111,%00110011,%11111110,%00000011,%11111111,%11110000,%01110100,%00000000
 .db %00000000,%00000000,%00000000,%00101010,%11100111,%11111100,%01111100,%00111111,%11110000,%00001100,%00000000
 .db %00000000,%00000000,%00000000,%01110101,%11001111,%11111111,%11000011,%10001111,%11110000,%00001110,%00000000
 .db %00000000,%00000000,%00000000,%11111011,%10011111,%11111111,%10001111,%11100011,%11110000,%00000110,%00000000
 .db %00000000,%00000000,%00000000,%11110101,%10111101,%11111110,%00011110,%10111001,%11110000,%00000110,%00000000
 .db %00000000,%00000000,%00000001,%11101011,%10111101,%11111110,%00111011,%01111000,%11110000,%00000000,%00000000
 .db %00000000,%00000000,%00001110,%11110101,%10111010,%11111111,%11111111,%10111100,%11111000,%00000000,%00000000
 .db %00000000,%00000000,%00110001,%10101011,%11111100,%11111111,%11111000,%00001111,%11111000,%00000000,%00000000
 .db %00000000,%00000000,%01000011,%11110000,%11111010,%01111111,%11000010,%00111111,%11111000,%00000000,%00000000
 .db %00000000,%00000000,%10000011,%10000011,%11111100,%01111110,%00011111,%00000001,%11111000,%00000000,%00000000
 .db %00000000,%00000000,%10000011,%11111100,%01111110,%01111110,%00111101,%10111000,%11111100,%00000000,%00000000
 .db %00000000,%00000000,%00000111,%00010011,%00111100,%11111111,%11111011,%00011111,%11111100,%00000000,%00000000
 .db %00000000,%00000000,%00000110,%11001111,%10011110,%11111111,%11111100,%00111111,%11111100,%00000000,%00000000
 .db %00000000,%00000000,%00001111,%00010011,%11011100,%11111111,%11000001,%11100111,%11111100,%00000000,%00000000
 .db %00000000,%00000000,%00001100,%11111001,%11011111,%11111111,%00001110,%10101000,%11111100,%00000000,%00000000
 .db %00000000,%00000000,%00011000,%00001111,%11011101,%11111111,%11111111,%11100111,%00111100,%00000000,%00000000
 .db %00000000,%00000000,%00111111,%11100111,%11111011,%11111111,%11111100,%00001011,%11111110,%00000000,%00000000
 .db %00000000,%00000000,%00100000,%11111111,%11110011,%11111111,%11100000,%11110101,%11111111,%00000000,%00000000
 .db %00000000,%00000000,%11111100,%00011111,%11111011,%11111111,%10001111,%01000000,%00011111,%00000000,%00000000
 .db %00000000,%00000111,%10000011,%11001111,%11110011,%11111111,%10011111,%11100011,%11100111,%10000000,%00000000
 .db %00000000,%00011110,%01111100,%01111111,%11111011,%11111111,%11111111,%00000011,%10110011,%11000000,%00000000
 .db %00000000,%01111000,%00000111,%11111111,%11110011,%11111111,%11111000,%01110101,%00011111,%11100000,%00000000
 .db %00000011,%11000101,%01111011,%11111111,%11111001,%11111111,%11100111,%11101101,%11101111,%11111000,%00000000
 .db %00000111,%11111111,%11111111,%11111111,%11111101,%11111111,%11001111,%11010000,%00000000,%11111110,%00000000
 .db %00011000,%00000000,%01111111,%11111111,%11111001,%11111111,%11111110,%00000101,%11111110,%00111111,%00000000
 .db %00111111,%11111111,%10001111,%11111111,%11111101,%11111111,%11111000,%01111110,%10101001,%10011111,%10000000
 .db %00101001,%00100100,%11110000,%11111111,%11111001,%11111111,%11111111,%11111111,%11111111,%11111111,%11100000
 .db %01010010,%01000100,%10011110,%01111111,%11111101,%11111111,%11111111,%11111111,%11000000,%00000111,%11110000
 .db %01010010,%01000100,%10010111,%00111111,%11111001,%11111111,%11111110,%00000000,%00000000,%00000000,%01111000
 .db %01001111,%11111111,%11100100,%10010101,%01010011,%11111111,%11000000,%00000000,%00011111,%11111100,%00011000
 .db %00111111,%11111111,%11111001,%00101010,%10101011,%11111111,%10000000,%00000111,%11110100,%00100011,%11001100
 .db %00000111,%11111111,%11111111,%11111111,%11110111,%11111111,%00001111,%11111100,%01000010,%00010001,%00111100
 .db %00000000,%00000000,%00000000,%00000000,%00000111,%11111101,%11101000,%10001000,%10000010,%00001000,%10010100
 .db %00000000,%00000000,%00000000,%00000000,%00000100,%00001110,%10001001,%00010000,%10000010,%00001000,%10001010
 .db %00000000,%00000000,%00000000,%00000000,%00000100,%00001001,%00010001,%00010000,%10000010,%00010000,%10001010
 .db %00000000,%00000000,%00000000,%00000000,%00000110,%00001000,%10010001,%00010000,%01000010,%11111111,%00010100
 .db %00000000,%00000000,%00000000,%00000000,%00000001,%11101000,%10001000,%10001000,%11111111,%11111111,%11111000
 .db %00000000,%00000000,%00000000,%00000000,%00000000,%00100100,%01000100,%01011111,%11111111,%11111111,%11110000
 .db %00000000,%00000000,%00000000,%00000000,%00000000,%00010010,%00100111,%11111111,%11111111,%11111111,%11100000
 .db %00000000,%00000000,%00000000,%00000000,%00000000,%00011111,%11111111,%11111111,%11111111,%11111111,%00000000
 .db %00000000,%00000000,%00000000,%00000000,%00000000,%00000011,%11111111,%11111111,%11111111,%11110000,%00000000

.end
