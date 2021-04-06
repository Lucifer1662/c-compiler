
.data
.globl hello
hello:
.string "Hi World\n"

.text
.global main
main:
    pushq   %rbp
    movq    %rsp,       %rbp
    movq    $hello,     %rdi
    call    puts
    movq    $0,         %rax
    leave
    ret
