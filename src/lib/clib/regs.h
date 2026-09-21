#pragma once

#include "stdint.h"

#pragma pack(push, 1)

typedef struct
{
    uint16_t flags;
    uint16_t es, ds;

    uint16_t di, si;
    uint16_t bp, sp;
    uint16_t bx, dx, cx, ax;

    uint16_t ip;
    uint16_t cs;
    uint16_t cpuFlags;
} REGS;

#pragma pack(pop)