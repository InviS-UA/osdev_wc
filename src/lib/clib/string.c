#include "string.h"
#include "stddef.h"

const char* _cdecl strchr(const char* str, char chr)
{
    if (str == NULL)
        return NULL;

    while (*str)
    {
        if (*str == chr)
            return str;

        ++str;
    }

    return NULL;
}

char* _cdecl strcpy(char* dst, const char* src)
{
    char* origDst = dst;

    if (dst == NULL)
        return NULL;

    if (src == NULL)
    {
        *dst = '\0';
        return dst;
    }

    while (*src)
    {
        *dst = *src;
        ++src;
        ++dst;
    }
    
    *dst = '\0';
    return origDst;
}

size_t _cdecl strlen(const char* str)
{
    size_t len = 0;

    while (*str)
    {
        len++;
        str++;
    }

    return len;
}

int _cdecl strcmp(const char* str1, const char* str2)
{
    while (*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }

    return (unsigned char)*str1 - (unsigned char)*str2;
}