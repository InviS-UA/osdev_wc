#pragma once

#include <clib/stdint.h>
#include <clib/regs.h>
#include "prf.h"
#include <defs.h>
#include <dev/device.h>

// Assembly interrupt handlers
extern void _cdecl int21_handler();

// C interrupt handlers
void _cdecl Int21Handler(REGS* regs);

// Initialization functions
void _cdecl init_interrupts();
void _cdecl init_kernel();

// Kernel entry point
void _cdecl DOSKernMain(uint16_t bootDrive);