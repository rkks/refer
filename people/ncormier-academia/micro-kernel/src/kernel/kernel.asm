;; 
;; kernel.asm
;; 
;; Made by nicolas
;; Mail   <n.cormier@gmail.com>
;;

	
[BITS 32]
[global kernel]
[extern kernel_main]

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Kernel entry point
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
kernel:
  call kernel_main ; jump to our C kernel entry point
  hlt
