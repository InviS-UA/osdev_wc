#include "prf.h"
#include <clib/stdarg.h>
#include <clib/stdbool.h>
#include "x86/vga.h"

void _cdecl putc(char c)
{
    VgaPutc(c);
}

void _cdecl puts(const char* str)
{
    while (*str)
    {
        putc(*str);
        str++;
    }
}

#define PRINTF_STATE_NORMAL         0
#define PRINTF_STATE_LENGTH         1
#define PRINTF_STATE_LENGTH_SHORT   2
#define PRINTF_STATE_LENGTH_LONG    3
#define PRINTF_STATE_SPEC           4

#define PRINTF_LENGTH_DEFAULT       0
#define PRINTF_LENGTH_SHORT_SHORT   1
#define PRINTF_LENGTH_SHORT         2
#define PRINTF_LENGTH_LONG          3

void printf_unsigned(unsigned long number, int radix);
void printf_signed(long number, int radix);

void _cdecl printf(const char* fmt, ...)
{
    int state = PRINTF_STATE_NORMAL;
    int length = PRINTF_LENGTH_DEFAULT;
    int radix = 10;
    bool sign = false;
    bool number = false;

    va_list args;
    va_start(args, fmt);

    while (*fmt)
    {
        switch (state)
        {
        case PRINTF_STATE_NORMAL:
            switch (*fmt)
            {
            case '%':
                state = PRINTF_STATE_LENGTH;
                break;
            default:
                putc(*fmt);
                break;
            }
            break;
        
        case PRINTF_STATE_LENGTH:
            if (*fmt == 'h')
            {
                length = PRINTF_LENGTH_SHORT;
                state = PRINTF_STATE_LENGTH_SHORT;
            }
            else if (*fmt == 'l')
            {
                length = PRINTF_LENGTH_LONG;
                state = PRINTF_STATE_LENGTH_LONG;
            }
            else
            {
                state = PRINTF_STATE_SPEC;
                continue;
            }
            break;
        
        case PRINTF_STATE_LENGTH_SHORT:
            if (*fmt == 'h')
            {
                length = PRINTF_LENGTH_SHORT_SHORT;
                state = PRINTF_STATE_SPEC;
            }
            else
            {
                state = PRINTF_STATE_SPEC;
                continue;
            }
            break;
        
        case PRINTF_STATE_LENGTH_LONG:
            state = PRINTF_STATE_SPEC;
            continue;
        
        case PRINTF_STATE_SPEC:
            switch (*fmt)
            {
            case 'c':
                putc((char)va_arg(args, int));
                break;
            case 's':
                puts(va_arg(args, const char*));
                break;
            case '%':
                putc('%');
                break;
            case 'd':
            case 'i':
                radix = 10;
                sign = true;
                number = true;
                break;
            case 'u':
                radix = 10;
                sign = false;
                number = true;
                break;
            case 'X':
            case 'x':
            case 'p':
                radix = 16;
                sign = false;
                number = true;
                break;
            case 'o':
                radix = 8;
                sign = false;
                number = true;
                break;
            }

            if (number)
            {
                if (sign)
                {
                    switch (length)
                    {
                    case PRINTF_LENGTH_SHORT_SHORT:
                    case PRINTF_LENGTH_SHORT:
                    case PRINTF_LENGTH_DEFAULT:
                        printf_signed(va_arg(args, int), radix);
                        break;
                    case PRINTF_LENGTH_LONG:
                        printf_signed(va_arg(args, long), radix);
                        break;
                    }
                }
                else
                {
                    switch (length)
                    {
                    case PRINTF_LENGTH_SHORT_SHORT:
                    case PRINTF_LENGTH_SHORT:
                    case PRINTF_LENGTH_DEFAULT:
                        printf_unsigned(va_arg(args, unsigned int), radix);
                        break;
                    case PRINTF_LENGTH_LONG:
                        printf_unsigned(va_arg(args, unsigned long), radix);                   
                        break;
                    }
                }
            }

            state = PRINTF_STATE_NORMAL;
            length = PRINTF_LENGTH_DEFAULT;
            radix = 10;
            sign = false;
            number = false;
            break;
        }

        fmt++;
    }

    va_end(args);
}

const char hexChars[] = "0123456789abcdef";

void printf_unsigned(unsigned long number, int radix)
{
    char buffer[32];
    int pos = 0;

    // convert number to ASCII
    do 
    {
        unsigned long rem = number % radix;
        number /= radix;
        buffer[pos++] = hexChars[rem];
    } while (number > 0);

    // print number in reverse order
    while (--pos >= 0)
        putc(buffer[pos]);
}

void printf_signed(long number, int radix)
{
    if (number < 0)
    {
        putc('-');
        printf_unsigned(-number, radix);
    }
    else printf_unsigned(number, radix);
}