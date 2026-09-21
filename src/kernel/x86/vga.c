#include "vga.h"
#include "io.h"
#include <clib/stdint.h>
#include <clib/stdbool.h>

const unsigned SCREEN_WIDTH = 80;
const unsigned SCREEN_HEIGHT = 25;
const uint8_t DEFAULT_COLOR = 0x7;

uint8_t far* VgaMemoryAddress = (uint8_t far*)0xB8000000;
int VgaX = 0, VgaY = 0;

void VgaPutChar(int x, int y, char c)
{
    VgaMemoryAddress[2 * (y * SCREEN_WIDTH + x)] = c;
}

void VgaPutColor(int x, int y, uint8_t color)
{
    VgaMemoryAddress[2 * (y * SCREEN_WIDTH + x) + 1] = color;
}

char VgaGetChar(int x, int y)
{
    return VgaMemoryAddress[2 * (y * SCREEN_WIDTH + x)];
}

uint8_t VgaGetColor(int x, int y)
{
    return VgaMemoryAddress[2 * (y * SCREEN_WIDTH + x) + 1];
}

void VgaSetCursor(int x, int y)
{
    int pos = y * SCREEN_WIDTH + x;

    x86_outb(0x3D4, 0x0F);
    x86_outb(0x3D5, (uint8_t)(pos & 0xFF));
    x86_outb(0x3D4, 0x0E);
    x86_outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void VgaClear()
{
    for (int y = 0; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            VgaPutChar(x, y, '\0');
            VgaPutColor(x, y, DEFAULT_COLOR);
        }

    VgaX = 0;
    VgaY = 0;
    VgaSetCursor(VgaX, VgaY);
}

void VgaScrollBack(int lines)
{
    for (int y = lines; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            VgaPutChar(x, y - lines, VgaGetChar(x, y));
            VgaPutColor(x, y - lines, VgaGetColor(x, y));
        }

    for (int y = SCREEN_HEIGHT - lines; y < SCREEN_HEIGHT; y++)
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            VgaPutChar(x, y, '\0');
            VgaPutColor(x, y, DEFAULT_COLOR);
        }

    VgaY -= lines;
}

void VgaPutc(char c)
{
    switch (c)
    {
        case '\n':
            VgaX = 0;
            VgaY++;
            break;
        case '\t':
            for (int i = 0; i < 4 - (VgaX % 4); i++)
                VgaPutc(' ');
            break;
        case '\r':
            VgaX = 0;
            break;
        default:
            VgaPutChar(VgaX, VgaY, c);
            VgaX++;
            break;
    }

    if (VgaX >= SCREEN_WIDTH)
    {
        VgaY++;
        VgaX = 0;
    }
    if (VgaY >= SCREEN_HEIGHT)
        VgaScrollBack(1);

    VgaSetCursor(VgaX, VgaY);
}