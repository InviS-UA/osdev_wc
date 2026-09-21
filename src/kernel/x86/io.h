#pragma once

#include <clib/stdint.h>

void _cdecl x86_outb(uint16_t port, uint8_t value);
uint8_t _cdecl x86_inb(uint16_t port);