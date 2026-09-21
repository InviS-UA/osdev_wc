bits 16

section _TEXT class=CODE

global _x86_outb
_x86_outb:
    push bp
    mov bp, sp

    mov dx, [bp + 4]
    mov al, [bp + 6]
    out dx, al

    mov sp, bp
    pop bp

    ret

global _x86_inb
_x86_inb:
    push bp
    mov bp, sp

    push dx

    mov dx, [bp + 4]
    xor ax, ax
    in al, dx

    pop dx

    mov sp, bp
    pop bp

    ret