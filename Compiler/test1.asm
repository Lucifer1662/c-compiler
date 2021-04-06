global  _main
extern  _printf
extern  _malloc
extern  _free
section .text
_main:

push ebp
mov ebp, esp
mov eax, 4
push dword eax
call _malloc
add esp, 4
sub esp, byte 4
mov [ebp -8], eax

mov eax, 4
push dword eax
call _malloc
add esp, 4

mov ebx, [ebp -8]
mov [ebx], eax


mov ebx, [ebp -8]
mov ebx, [ebx]

mov eax, 5
mov [ebx], eax
mov eax, [ebp -8]
mov eax, [eax]

push dword 5
push dword _SL1599722810
call _printf
add esp, 8
push dword [ebp -8]
call _free
add esp, 4
pop ebp
ret
section .data
_SL1599722810 db '%d',10,0
__floatControl dw 0