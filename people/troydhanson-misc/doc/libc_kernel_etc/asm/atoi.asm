[SECTION .text]; 

extern printf
extern atoi
global main

main:
  push ebp
  mov ebp,esp; bottom of stack becomes top of new stack frame
  push ebx
  push esi  ; save sacred registers
  push edi
  ; end boilerplate

  push dword aint     ; we're going to call atoi("123")
  call atoi           ; output is in EAX (123 == 0x7b)
  add esp, byte 4     ; clean up stack

  ; print it
  push eax
  push intformat
  call printf
  add esp, byte 8     ; clean up stack (assuming 32 bit pointer size)
  
  ; resume boilerplate
  pop edi
  pop esi
  pop ebx
  mov esp,ebp 
  pop ebp
  ret

[SECTION .data]
intformat: db "atoi result: eax is %d",10,0
aint: db "123",0

