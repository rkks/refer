[SECTION .text]

global main
extern printf

main:
  ; create new stack frame
  push ebp
  mov ebp, esp
  ; preserve sacred registers
  push ebx
  push esi
  push edi
  ; end boilerplate

  ;  stack has this layout now:

  ; ebp+16  envp
  ; ebp+12  argv
  ; ebp+ 8  argc 
  ; ebp+ 4  return address
  ; ebp     ebp             <-- ebp
  ; ebp- 4  ebx
  ; ebp- 8  esi
  ; ebp-12  edi             <-- esp

  mov ebx, [ebp+16]   ; envp
 next:
  mov ecx, [ebx]
  jecxz done

  push dword ecx;
  push dword fmt;
  call printf
  add esp, byte 8;     ; clean up stack

  add ebx, 4          ; next env variable
  jmp next



 done:
  ; boilerplate 
  pop edi
  pop esi
  pop ebx
  mov esp, ebp
  pop ebp
  ret

[SECTION .data]
fmt  db "%s",10,0
