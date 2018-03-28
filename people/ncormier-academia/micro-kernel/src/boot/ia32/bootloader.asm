;; 
;; bootloader.asm
;; 
;; Made by nicolas
;; Mail   <n.cormier@gmail.com>
;;

	
[BITS 32]
[global bootloader]
[extern bootloader_main]

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Bootloader entry point
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
bootloader:
  call bootloader_main ; jump to our C bootstrap entry point
  hlt
