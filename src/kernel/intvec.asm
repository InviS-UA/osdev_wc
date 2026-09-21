bits 16

section _TEXT class=CODE

%macro IntHandler 2
    global %1
    extern %2

%1:
    pusha
    push ds
    push es
    pushf

    ; Передаємо C-функції pointer на контекст
    mov ax, sp
    push sp
    call %2
    add sp, 2

    popf
    pop es
    pop ds
    popa
    iret
%endmacro

IntHandler _int21_handler, _Int21Handler