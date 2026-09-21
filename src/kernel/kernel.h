#pragma once

#include <clib/stdint.h>
#include <clib/regs.h>
#include "prf.h"
#include <defs.h>
#include <dev/device.h>

// Assembly wrapp handler
extern void _cdecl int21_handler();

void _cdecl init_interrupts();

// C interrupt function
void _cdecl Int21Handler(const REGS* regs);

void _cdecl DOSKernMain(uint16_t bootDrive);