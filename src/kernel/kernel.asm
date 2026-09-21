bits 16

section _START class=CODE

extern _DOSKernMain

global _start
_start:
    cli
    mov [BootDrive], dl

    ; setup segment registers
    xor ax, ax
    mov ds, ax
    mov es, ax

    ; setup stack
    mov ss, ax
    mov sp, 0xFFF0
    mov bp, sp
    sti

    ; expect boot drive in dl, send it as argument to cstart function
    xor dx, dx
    mov dl, [BootDrive]
    push dx
    call _DOSKernMain

    cli
    hlt

BootDrive: db 0