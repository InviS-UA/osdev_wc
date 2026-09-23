#include "kernel.h"
#include "x86/vga.h"
#include "dosalloc.h"
#include <clib/memory.h>
#include "prf.h"
#include <defs.h>
#include <dev/device.h>

void _cdecl DOSKernMain(uint16_t bootDrive)
{
    init_kernel();

    const int size1 = 1024;
    const int size2 = 101;
    const int size3 = 556;
    int far* ptr1;
    int far* ptr2;
    int far* ptr3;

    ptr1 = DosAlloc(size1 * sizeof(int));
    ptr2 = DosAlloc(size2 * sizeof(int));
    ptr3 = DosAlloc(size3 * sizeof(int));

    for (int i = 0; i < size1; i++)
        ptr1[i] = i + 1;

    for (int i = 0; i < size2; i++)
        ptr2[i] = i + 1;

    for (int i = 0; i < size3; i++)
        ptr3[i] = i + 1;
    
    DosHeapDump();

    DosFree(ptr1);
    DosFree(ptr2);
    DosFree(ptr3);
    
    ptr1 = DosAlloc(size1 * sizeof(int));
    ptr2 = DosAlloc(size2 * sizeof(int));

    DosFree(ptr1);
    DosFree(ptr2);

    DosHeapDump();

    for (;;);
}

void _cdecl init_kernel()
{
    init_interrupts();
    DosInitHeap((void far*)0x10000000, 16 * 1024);
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