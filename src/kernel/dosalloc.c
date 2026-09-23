#include "dosalloc.h"
#include "prf.h"
#include "defs.h"

#define BLOCK_SIZE sizeof(MemoryBlock)

static MemoryBlock far* g_HeapStart;
static MemoryBlock far* g_HeapEnd;

static MemoryBlock far* DosNextBlock(MemoryBlock far* block)
{
    return (MemoryBlock far*)((uint8_t far*)block + BLOCK_SIZE + block->m_size);
}

static uint32_t DosTotalFree()
{
    MemoryBlock far* block = g_HeapStart;
    uint32_t total = 0;

    while (block < g_HeapEnd)
    {
        if (block->m_free) total += block->m_size;
        if (block->m_last) break;

        block = DosNextBlock(block);
    }

    return total;
}

static void DosMergeBlocks()
{
    MemoryBlock far* block = (MemoryBlock far*)g_HeapStart;
    MemoryBlock far* next;

    while (block < g_HeapEnd)
    {
        if (block->m_last) break;

        next = DosNextBlock(block);

        if (block->m_free && next->m_free)
        {
            block->m_size += next->m_size + BLOCK_SIZE;
            block->m_last = next->m_last;

            continue;
        }

        block = next;
    }
}

void DosInitHeap(void far* address, uint32_t size)
{
    MemoryBlock far* block = (MemoryBlock far*)address;

    if (size == 0)
        return;

    g_HeapStart = block;
    g_HeapEnd = (MemoryBlock far*)((uint8_t far*)address + size);

    block->m_last = true;
    block->m_free = true;
    block->m_size = size - BLOCK_SIZE;
}

void far* DosAlloc(uint32_t size)
{
    MemoryBlock far* block = g_HeapStart;

    if (size == 0) return NULL;

    while (block < g_HeapEnd)
    {
        if (block->m_free && block->m_size >= size)
        {
            uint32_t oldSize = block->m_size;

            if (block->m_size < size + BLOCK_SIZE + 1)
            {
                block->m_size = size;
                block->m_free = false;

                return (void far*)((uint8_t far*)block + BLOCK_SIZE);
            }

            MemoryBlock far* newBlock = (MemoryBlock far*)((uint8_t far*)block + BLOCK_SIZE + size);
            newBlock->m_last = block->m_last;
            newBlock->m_free = true;
            newBlock->m_size = block->m_size - BLOCK_SIZE - size;

            block->m_last = false;
            block->m_free = false;
            block->m_size = size;

            return (void far*)((uint8_t far*)block + BLOCK_SIZE);
        }

        block = DosNextBlock(block);
    }

    return NULL;
}

int DosFree(void far* ptr)
{
    if (!ptr) return -1;

    MemoryBlock far* block = (MemoryBlock far*)((uint8_t far*)ptr - BLOCK_SIZE);

    if (!block || block->m_free) return -1;

    block->m_free = true;

    DosMergeBlocks();

    return 0;
}

void DosHeapDump()
{
    MemoryBlock far* block = g_HeapStart;

    printf("\nMemory Dump Chain\n");
    printf("-------------------------------\n");

    while (block < g_HeapEnd)
    {
        printf("BLOCK=%x:%x LAST=%d SIZE=%lu ",
               FP_SEG(block), FP_OFF(block), block->m_last, block->m_size);
        
        if (block->m_free) printf("FREE ");
        
        printf("\n");

        if (block->m_last) break;

        block = DosNextBlock(block);
    }

    printf("-------------------------------\n");
    printf("Block size: %u bytes\n", BLOCK_SIZE);
    printf("Total free: %lu bytes\n", DosTotalFree());
}