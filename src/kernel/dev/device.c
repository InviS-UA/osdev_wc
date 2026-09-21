#include "device.h"
#include <clib/string.h>

static Device* devices[DEV_MAX];

size_t _cdecl ConsoleRead(Device* dev, void far* buff, size_t size)
{
    return size;
}

size_t _cdecl ConsoleWrite(Device* dev, const void far* buff, size_t size)
{
    const uint8_t far* u8Buff = (const uint8_t far*)buff;

    for (size_t i = 0; i < size; i++)
    {
        char c = u8Buff[i];

        _asm {
            mov ah, 0Eh
            mov al, c
            mov bh, 0
            int 10h
        };
    }

    return size;
}

static Device CON =
{
    "CON",
    DEV_READ | DEV_WRITE,
    ConsoleRead,
    ConsoleWrite,
    NULL
};

static Device DevNull =
{
    "NUL",
    DEV_READ | DEV_WRITE,
    NULL,
    NULL,
    NULL
};

void DevInit()
{
    for (int i = 0; i < DEV_MAX; i++)
        devices[i] = &DevNull;

    devices[DEV_STDOUT] = &CON;
    devices[DEV_STDIN] = &CON;
    devices[DEV_STDERR] = &CON;
}

int DevOpen(const char* name)
{
    if (!name)
        return -1;

    for (int i = 0; i < DEV_MAX; i++)
        if (strcmp(devices[i]->name, name) == 0)
            return i;

    return -1;
}

int DevClose(int handle)
{
    if (handle < 0 || handle >= DEV_MAX || handle < 3)
        return -1;

    if (!devices[handle])
        return -1;

    devices[handle] = NULL;

    return 0;
}

size_t DevRead(int handle, void far* buff, size_t size)
{
    Device* device;

    if (handle < 0 || handle >= DEV_MAX || !devices[handle] || !(devices[handle]->flags & DEV_READ))
        return 0;

    device = devices[handle];

    if (!device->read)
        return 0;

    return device->read(device, buff, size);
}

size_t DevWrite(int handle, const void far* buff, size_t size)
{
    Device* device;

    if (handle < 0 || handle >= DEV_MAX || !devices[handle] || !(devices[handle]->flags & DEV_WRITE))
        return 0;

    device = devices[handle];

    if (!device->write)
        return 0;

    return device->write(device, buff, size);
}