#pragma once

typedef unsigned char* va_list;

#define va_start(vl, last) ( (vl) = (char*)&(last) + ((sizeof(last) + sizeof(int) - 1) & -sizeof(int)) )
#define va_arg(vl, type) (*(type*)(((vl) += ((sizeof(type) + sizeof(int) - 1) & -sizeof(int))) - ((sizeof(type) + sizeof(int) - 1) & -sizeof(int))))
#define va_end(vl)