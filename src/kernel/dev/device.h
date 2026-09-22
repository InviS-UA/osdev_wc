#pragma once

#include <clib/stdint.h>
#include <clib/stddef.h>

#define DEV_STDIN 0
#define DEV_STDOUT 1
#define DEV_STDERR 2

#define DEV_MAX 16

#define DEV_READ 0x01
#define DEV_WRITE 0x02

typedef struct Device Device;

typedef size_t (_cdecl *DevReadFunc)(Device* dev, void far* buff, size_t size);
typedef size_t (_cdecl *DevWriteFunc)(Device* dev, const void far* buff, size_t size);

struct Device
{
    const char* name;
    uint16_t flags;
    int opened;

    DevReadFunc read;
    DevWriteFunc write;

    void* data;
};

void DevInit();

int DevOpen(const char* name);
int DevClose(int handle);

size_t DevRead(int handle, void far* buff, size_t size);
size_t DevWrite(int handle, const void far* buff, size_t size);