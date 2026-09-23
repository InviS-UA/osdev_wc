#pragma once

#include <clib/stdint.h>
#include <clib/stddef.h>
#include <clib/stdbool.h>

#pragma pack(push, 1)

typedef struct {
    bool m_free;
    bool m_last;
    size_t m_size;
} MemoryBlock;

#pragma pack(pop)

void DosInitHeap(void far* address, size_t size);
void far* DosAlloc(size_t size);
int DosFree(void far* ptr);
void DosHeapDump();