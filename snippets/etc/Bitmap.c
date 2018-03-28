Hi,

  I did a lot of graphics work at one point for a project I was doing.
I've taken the headers and source for my mouse driver and hacked them
down to the few routines dealing with video saves/restores.
  The routines you want to look at are "SaveVideo" and "RestoreVideo". I
also included the code for "DrawCursor" for your reference.
Note that this code assumes the bitmap starts on a byte
boundary; e.g. that the x position is a multiple of 8.  This is because
you read a byte at a time, which is 8 pixels in mode 12.  If you want
to read/write arbitrary bits you have to mask out the bits you don't want,
and possibly rotate the bits if you want to put them somewhere other than
where you got them.
  There are 4 bitplanes in mode 12; together they select the color.  As I
say you read/write one at a time until you've done them all.

  This program sets up the graphics controller/sequencer appropriately,
using port commands.  Screen data is then read/written to the buffer at
a000:0.  In mode 12, one raster row is 80 bytes (640 bits).
  I have a bunch of macros (included) which do an "out dx,ax".  This is
because most graphics controller commands take up two ports.  The address
of the port (in dx) tells which chip you are accessing, graphics controller,
sequencer, attribute controller, etc.  The first port (at dx+0) is
generally a command port in which you specify a register to read/write.
The second port (at dx+1) is where you specify the value to put in the
register.  Thus:


  mov dx,03c4h   ; Sequencer
  mov al,2  	 ; Plane select reg
  mov ah,8	 ; Planes to select, bit 3 is set so select plane 3
  out dx,ax      ; perform command

uses the sequencer to select that only one plane should be
enabled for write.  On the other hand:

  mov dx,03ceh	; Graphics Controller
  mov al,4	; Plane select reg
  mov ah,3	; Plane to select = 3
  out dx,ax	; perform command

is used to tell the graphics controller which plane you want to show up
in the graphics buffer (a000:0) when you do a read.


One thing you might find handy in your app that I don't need to do is:

  mov dx,03ceh  ; GCR
  mov al,8	; Bit mask register
  mov ah,xxx	; Bits to enable
  out dx,ax     ;

where 'xxx' is 0xff for enable all 8 bits in a byte for write,
0x00 for disable all 8 bits in a byte for write, 0x80 to enable only
the leftmost bit, etc.  You'd use this sequence whenever writing the
leftmost or rightmost byte of your bitmap, so as not to disturb bits
that are outside the bitmap.  Be sure to set it back to 0xff though for
the middle bytes!

This is part of a protected mode 386 app; the segment DSABS is set to map
the pc memory linearly into the 386 address map.  Thus address 0 in DSABS
is 0000:0000 in 16-bit terminology; the graphics memory at a000:0000
shows up at a0000 in DSABS.

If you want to know exactly what is happening/what the registers I'm
using are, let me refer you to a couple of books:

"Programmer's Guide to PC & PS/2 Video Systems", Richard Wilton,
Microsoft Press ISBN 1-55615-103-9

"Programmer's Guide to the EGA and VGA Cards", Richard F. Ferraro,
Addison-Wesley ISBN 0-201-57025-4

Both books give source code for a variety of applications, including
text output, lines/circles/etc, bitmaps.  I think the first book is
easier to understand, but I like the second because it describes each of
the registers on all the graphics chips in detail.
  
E-Mail me if you've got any questions.

David Lindauer
---------------------------------------------------------------------------

LinesPerChar	=	16
CharsPerLine	=	80
LinesPerScreen	=	30
CharsPerScreen	=	(CharsPerLine * LinesPerScreen)

BLACK		=	0
DGRAY		=	8
LGRAY		=	7
WHITE		=	15

GCR	= 03ceh
SCR	= 03c4h
GRAPHBASE = 0a0000h	; for 16-bit prog this would be a000:0

;
; Get graphics control reg
;
MACRO	GETGCR
	mov	dx,GCR		; Graphics control regsiter
ENDM
;
; Get seqencer control reg
;
MACRO	GETSCR
	mov	dx,SCR		; Sequencer control reg
ENDM	GETSCR
;
; Set Graphics Ctrller plane
;
MACRO	GRAPHICSPLANE
	mov	al,4		; Plane select reg
	out	dx,ax		; Select the plane
ENDM	GRAPHICSPLANE
;
; Set Sequencer plane
;
MACRO	SEQUENCERPLANE
	mov	al,2		; Plane select register
	out	dx,ax		; Select the plane
ENDM	SEQUENCERPLANE
;
; Reset sequencer plane select
;
MACRO	SPLANERESET
	mov	ax,0f02h	; Plane select default value (all planes)
	out	dx,ax		; Reset it
ENDM	SPLANERESET
;
; Set Replace mode
;
MACRO	WRFUNCREPLACE
	mov	ax,3 		; Replace mode, DRR (data rotate reg)
	out	dx,ax		; Set it
ENDM	WRFUNCREPLACE
;
; Set AND mode
;
MACRO	WRFUNCAND
	mov	ax,0803h	; AND mode, DRR
	out	dx,ax		; Set it
ENDM	WRFUNCAND
;
; Set XOR mode
;
MACRO	WRFUNCXOR
	mov	ax,1803h	; XOR mode, DRR
	out	dx,ax		; Set it
ENDM	WRFUNCXOR
;
; Set Read mode 1
;
MACRO	RDMODE1
	mov	ax,0805h	; Select read mode 1, write mode 0
	out	dx,ax
ENDM	RDMODE1
;
; Set write mode 3
;
MACRO	WRMODE3
	mov	ax,0305h	; Set read mode 0, write mode 3
	out	dx,ax
	mov	ax,0f01h	; Set the Enable set/reset register
	out	dx,ax
ENDM
;
; Set write mode 0
;
MACRO	WRMODE0
	mov	ax,5h		; Set read mode 0, write mode 0 (default)
	out	dx,ax
	mov	ax,1h		; Clear the enable set/reset register
	out	dx,ax
ENDM
;
; Setwrite mode 2
;
MACRO	WRMODE2
	mov	ax,0205h	; Set read mode 0, write mode 2
	out	dx,ax
ENDM
;
; Set the color, Mode 3 write
;
MACRO	SETCOLOR	color
	mov	ah,color	; Get color
	sub	al,al		; Selecte set/reset register
	out	dx,ax
ENDM
;
; Initialize background color, mode 3 write
;
MACRO	PUTBG
	mov	[byte ptr es:edi],-1 ; Write the color out
	mov	al,[byte ptr es:edi] ; Read the planes into the plane maps
ENDM
;
; Set the color, mode 2 write
;
MACRO	PUTCOLOR	latch
	ifnb	<latch>
	mov	ah,[es:edi]
	endif
	stosb
ENDM
;
; Set the bitmask, mode 2 write
;
MACRO	SETMASK	mask
	ifdif	<mask>,<ah>
	mov	ah,mask
	endif
	mov	al,8
	out	dx,ax
ENDM
; Mouse.asm
;
	IDEAL
	P386

include "segs.asi"
include "os.asi"
include "serial.asi"
include "mouse.asi"
include "dispatch.ase"
include "prints.ase"
include "vgaini.ase"
include "video.ase"
include "vga.asi"
include "vga.mac"

AGILER = 1
FL_SHOWN = 1
FL_INUSE = 0

CURSORLINES = 16
CURSORCOLS = 3		; Bytes

	PUBLIC	AttachMouse, DetachMouse, MouseIntr, MouseHandler
	PUBLIC	HideMouse, ShowMouse
SEGMENT	seg386data
dgrcurs	db	03fh, 0ffh, 01fh, 0ffh, 00fh, 0ffh, 007h, 0ffh ; And mask
	db	003h, 0ffh, 001h, 0ffh, 000h, 0ffh, 000h, 07fh
	db	000h, 03fh, 000h, 01fh, 001h, 0ffh, 010h, 0ffh
	db	030h, 0ffh, 0f8h, 07fh, 0f8h, 07fh, 0fch, 07fh
	; (append 0s)

	db	000h, 000h, 040h, 000h, 060h, 000h, 070h, 000h ; XOR mask
	db	078h, 000h, 07ch, 000h, 07eh, 000h, 07fh, 000h
	db	07fh, 080h, 07ch, 000h, 06ch, 000h, 046h, 000h
	db	006h, 000h, 003h, 000h, 003h, 000h, 000h, 000h
	; ( append 0s)
dhotspot db	0,0
inuse	dd	0
port	dd	0
col	dd	0
row	dd	0
colinscale dd	0
coloutscale dd	0
rowinscale dd	0
rowoutscale dd	0
maxrow	dd	0
maxcol	dd	0
uhot	dd	0
lhot	dd	0
state	db	0
leftcount db	0
rightcount db	0
	align
overwritebuffer db 3*16*4 DUP (0)
grcursand	db	3*16 DUP (0)	; Init code (not shown) moves
grcursxor	db	3*16 DUP (0)	; dgrcurs here.
ENDS	seg386data


SEGMENT seg386
;
; Get the Graphics Cursor Address, Graphics Mode
;   Also load number of lines, number of cols, and mask offset
;
PROC	GraphicsCursorAddress
	mov	eax,[row]		; ROW
	mov	ch,CURSORLINES		; CH = lines to loop through
	mov	cl,CURSORCOLS		; CL = cols to loop through
	sub	esi,esi			; ESI = starting cursor bitmask offset
	mov	ebx,CharsPerLine	; EBX = chars/line
	mul	bx			; EAX = row * chars/line
	shl	edx,16			; Extend to 32 bits
	or	eax,edx			;
	sub	eax,[uhot]		;    - hotspot
	jns     gca_fulllines		; If positive, draw whole thing
gca_lp:
	dec	ch			; Else dec lines to draw
	add	esi,CURSORCOLS		; Point to next line in bitmap
	add	eax,ebx			; Next line
	js	gca_lp			; Loop whiel below screen
gca_fulllines:
	mov	ebx,[maxcol]		; Get max col
	inc	ebx			;
	sub	ebx,[col]		; Subtract col/8+1
	shr	ebx,3			;
	inc	ebx			;
	cmp	bl,cl			; See if clip at right
	jnc	short gca_allcols	; No, do all cols
	mov	cl,bl			; Else get cols to do
gca_allcols:
	mov	ebx,[col]		; Get col
	shr	ebx,3
	add	eax,ebx			; Add row and col
	mov	edi,eax			; ESI gets screen address
	add	edi,GRAPHBASE		;
	mov	eax,[col]	; See if need to shift left by columns
	sub	eax,[lhot]	;
	js	short gca_nohot	; NO, continue
	mov	eax,[col]	; Find out how far to shift
	and	eax,7		;
	sub	eax,[lhot]	;
	jns	gca_nohot	;
gca_hot:
	dec	edi		; Shift left one column
	inc	cl       	; Inc amount to claim
	add	eax,8		; See if done
	js	short gca_hot	;
gca_nohot:
	cmp	cl,3
	jc	short gca_ok
	mov	cl,3
gca_ok:
	ret
ENDP	GraphicsCursorAddress
;
; Save memory under cursor
;
PROC	SaveVideo
	call	GraphicsCursorAddress	; Get cursor address
	push	ds			; DS = ABS data
	push	DSABS			;
	pop	ds			;
	mov	esi,edi			; ESI = screen buffer
	mov	edi,offset overwritebuffer; EDI = overwrite buffer
	GETGCR				; Get the GCR
	; assumes  Read mode 0, which is the default
	mov	ah,3			; Plane 3
	mov	bl,cl			; bl = cols (in bytes)
sv_plane:
	GRAPHICSPLANE			; Select the graphics plane
	push	ecx			; Save rows
	push	esi			; Save screen buffer
sv_row:
	mov	cl,bl			; Get cols
	push	esi			; Save row ptr
sv_col:
	movsb				; Move a byte
        dec	cl			; Next col
	jnz	sv_col			;
	pop	esi			; Restore row ptr
	add	esi,CharsPerLine	; Next row
	dec	ch			;
	jnz	sv_row			;
	pop	esi			; Restore screen buffer
	pop	ecx			; Restore rows
	dec	ah			; Next plane
	jns	sv_plane		;
	pop	ds			; Restore DS
	ret
ENDP	SaveVideo
;
; Restore memory under cursor
;
PROC	RestoreVideo
	call	GraphicsCursorAddress	; Get cursor
	push	es			; ES = DSABS
	push	DSABS			;
	pop	es			;
	mov	esi,offset overwritebuffer; Get save buffer
	GETGCR				; Load GCR
	RDMODE1				; Put us in Read mode 1/write mode 0
	mov	ah,8			; Plane 3
	mov	bl,cl			; Columns in bl (in bytes)
	GETSCR				; Get Sequencer
rs_plane:
	SEQUENCERPLANE			; Set the plane at the sequencer
	push	edi			; Save screen address
	push	ecx			; And rows
rs_row:
	mov	cl,bl			; Load cols
	push	edi			; Save current row
rs_col:
	movsb				; Move a byte
	dec	cl			; Next col
	jnz	rs_col			;
	pop	edi			; Restore current row
	add	edi,CharsPerLine	; Got to next row
	dec	ch			; Next row
	jnz	rs_row			;
	pop	ecx			; Restore rows
	pop	edi			; Restore screen address
	shr	ah,1			; Next plane
	jnc	rs_plane		; Loop through planes
	SPLANERESET			; Reset sequencer for all planes
	GETGCR				; Load graphics controller
	WRMODE0				; Read mode 0
	pop	es			; Restore es
	ret
ENDP	RestoreVideo
;
; Apply a graphics cursor mask
;
PROC	MaskCursor
	cld			; This CLD must be here in case interrupted an STD
mc_row:
	lodsb			; Get the three byte mask
	mov	bh,al		;
	lodsb			;
	mov	ah,al		;
	lodsb			;
	push	ecx		; See if undeflow
	pushfd			;
	mov	ecx,[col]	;
	sub	ecx,[lhot]	;
	js	short mc_leftshift
	and	ecx,7		;
	popfd			; Restore what to shift in from left
	jecxz	mc_noshift	; Don't shift if none
mc_rightshift:
	pushfd			; Save shift bit
	rcr	bh,1		; Do one shift right
	rcr	ax,1		;
	popfd			; Restore shift bit
	loop	mc_rightshift	; shift
	jmp	short mc_noshift
mc_leftshift:
	neg	ecx		; Negate shift count
	popfd			; Restore bit to shift in
mc_shiftleft:
	pushfd			; Save shift bit
	rcl	ax,1		; Do one left shift
	rcl	bh,1		;
	popfd			; Restore shift bit
	loop	mc_shiftleft	; More shift
mc_noshift:
	pop	ecx		; Restore row & col shifts
	pushfd			; Save shift bit
	mov	cl,bl		; Get cols
	push	edi		; Save screen pos
	push	eax		; Save bit pattern
	mov	al,[es:edi]	; Apply mask to first byte
	mov	[es:edi],bh	;
	pop	eax		;
	inc	edi		; Second byte
	dec	cl		; See if more than one byte to display
	jz	short mc_donerow; Quit if not
	mov	bh,[es:edi]	; Apply second byte of mask
	mov	[es:edi],ah	;
	inc	edi		; Point to third byte
	dec	cl		; See if to do a third byte
	jz	short mc_donerow; If not
	mov	bh,[es:edi]	; Apply third byte
	mov	[es:edi],al	;
mc_donerow:
	pop	edi		; Point to next row
	add	edi,CharsPerLine;
	popfd			; Restore shift bit
	dec	ch		; Next row
	jnz	mc_row		;
	ret
ENDP	MaskCursor
; Draw the graphics cursor
;
PROC	DrawCursor
	call	GraphicsCursorAddress	; Get cursor address
	push	es			; ES = absolute seg
	push	DSABS			;
	pop	es			;
	GETGCR				; Get control reg
	mov	bl,cl			; bl = cols
	push	edi			; Save regs
	push	ebx			;
	push	ecx			;
	push	esi                     ;
	add	esi,offset grcursand	; Cursor mask
	WRFUNCAND			; Anding to bit planes
	stc				; Shift in a one bit
	call	MaskCursor		; Apply the and mask
	pop	esi
	pop	ecx			; Restore position
	pop	ebx			;
	pop	edi			;
	add	esi,offset grcursxor	; Cursor mask
	WRFUNCXOR			; XORing to bit planes
	clc				; Shift in a 0 bit
	call	MaskCursor		; Apply the xor mask
	pop	es			; Restore absolute seg
	WRFUNCREPLACE			; Put us back in replace mode
	ret
ENDP	DrawCursor
ENDS	seg386
END
