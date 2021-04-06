global  _main
extern  _printf
extern  _malloc
extern  _free
section .text
_main:

push ebp
mov ebp, esp
push dword 4
call _malloc
add esp, 4

push eax


push dword 4
call _malloc
add esp, 4

mov ebx, 

mov [ebp -8], eax

mov dword [eax], 3


mov ecx, [ebp -8]
mov ecx, [ecx]

push dword ecx
push dword _SL1599722810
call _printf
add esp, 8


pop ebp
ret
section .data
_SL1599722810 db '%d',10,0
__floatControl dw 0