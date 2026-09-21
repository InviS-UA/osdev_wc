#pragma once

#include "stddef.h"

const char* _cdecl strchr(const char* str, char chr);
char* _cdecl strcpy(char* dst, const char* src);
size_t _cdecl strlen(const char* str);
int _cdecl strcmp(const char* str1, const char* str2);