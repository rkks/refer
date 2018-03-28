;; 
;; bootstrap.asm
;; 
;; Made by nicolas
;; Mail   <n.cormier@gmail.com>
;;

	
[BITS 16]
[ORG 0x7C00]

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Read and copy kernel at 1000:0000
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
start:
  mov ax, start			; Setup segment register
  mov es, ax			
  mov ds, ax
  mov ss, ax
  mov sp, ax

reset:				; Reset harddisk
  mov ax, 0
  mov dl, 80h
  int 13h
  jc reset
    
read:				; Read kernel from harddisk
  mov bx, 0x1000
  mov es, bx    	  	; Seg
  mov bx, 0       		; Offset - So destination address is 0000:1000
  mov ah, 2
  mov al, 128      		; Number of sectors to read (1 sector = 512 bytes)
  mov ch, 0       		; Cylinder number
  mov cl, 2       		; Sector number
  mov dh, 0       		; Head number
  mov dl, 80h     		; Drive number (hda)
  int 13h
  jc read

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Switch to protected mode
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
cli 				; Disable intr
xor ax, ax
mov ds, ax 			; Clear ds
lgdt [gdtr] 			; Setup gdt 
	;; Active protected mode
mov eax, cr0
or eax, 1
mov cr0, eax

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Now we are in protected mode
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;; Refresh registers
mov ax, 10h
mov ds, ax
mov ss, ax
	;; Setup temporary stack far away;
mov esp, 090000h
mov ebp, 090000h
	;; Jump to bootloader entry point
jmp dword 08h:10000h

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; GTD declaration
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
gdt:
  gdt_null:
    dd 0
    dd 0

  gdt_code:
    dw 0FFFFh			; Limit1
    dw 0			; Base1 (16bits)
    db 0			; Base2 (8bits)
    db 10011010b		; p=ispresent dpl(2bits)=0ring s=code/data Type(4bits)=Code,read/exec
    db 11011111b		; g=page_granularity d/b=32bit_segment l=isnot_64bit_segment avl Limit2(4 bits)
    db 0			; Base 3 (8bits)

  gdt_data:
    dw 0FFFFh			
    dw 0
    db 0
    db 10010010b		; Type=Data,read/write
    db 11011111b
    db 0

gdt_end:
gdtr:
  dw gdt_end - gdt - 1
  dd gdt

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Helpers
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
print_message:            ; Dump ds:si to screen.
  lodsb                   ; load byte at ds:si into al
  or al,al                ; test if character is 0 (end)
  jz print_message_done
  mov ah,0eh              ; put character
  mov bx,0007             ; attribute
  int 0x10                ; call BIOS
  jmp print_message
print_message_done:
  ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Bootstrap sig
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
times 510 - ($ - $$) db 0
dw 0xAA55
