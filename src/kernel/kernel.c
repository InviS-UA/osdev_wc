#include "kernel.h"
#include "x86/vga.h"

void _cdecl DOSKernMain(uint16_t bootDrive)
{
    init_kernel();

    printf("Hello from kernel!\n");

    for (;;);
}

void _cdecl init_kernel()
{
    init_interrupts();
    DevInit();
    VgaClear();
}

void _cdecl init_interrupts()
{
    uint16_t far* vec = (uint16_t far*)0x0;
    vec[0x42] = FP_OFF(int21_handler);
    vec[0x43] = FP_SEG(int21_handler);
}

static size_t FarStrlen(const char far* str)
{
    size_t len = 0;

    while (*str)
    {
        len++;
        str++;
    }

    return len;
}

void _cdecl Int21Handler(REGS* regs)
{
    const uint8_t ah = (uint8_t)(regs->ax >> 8);

    switch (ah)
    {
        case 0x02:
        {
            uint8_t c = (uint8_t)(regs->dx & 0xFF);
            DevWrite(0, &c, 1);
            break;
        }
        case 0x09:
        {
            const char far* str = (const char far*)MK_FP(regs->ds, regs->dx);
            DevWrite(0, str, FarStrlen(str));
            break;
        }
    }
}