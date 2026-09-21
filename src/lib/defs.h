#pragma once

#define FP_OFF(addr) (uint16_t)((uint32_t)(addr) & 0xFFFF)
#define FP_SEG(addr) (uint16_t)((uint32_t)(addr) >> 16)

#define MK_FP(seg, off) (void far*)((uint32_t)(seg << 16) + off)