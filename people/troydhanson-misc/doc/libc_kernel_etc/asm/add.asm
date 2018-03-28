[SECTION .text]; 

global add

add:
  push ebp
  mov ebp,esp; bottom of stack becomes top of new stack frame
  push ebx
  push esi
  push edi

  ; parent frame has our inputs at ebp+{8,12} and return address at ebp+4
  mov eax, [ebp+8]
  mov ecx, [ebp+12]
  add eax,ecx; c calling convention returns 32-bit output in eax

  ; resume boilerplate
  pop edi
  pop esi
  pop ebx
  mov esp,ebp 
  pop ebp
  ret

[SECTION .data]

