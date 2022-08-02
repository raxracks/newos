#include <stdint.h>

// https://wiki.osdev.org/Meaty_Skeleton#libc.2Fstring.2Fmemcmp.c

int memcmp(const void *aptr, const void *bptr, uint32_t size)
{
    const unsigned char *a = (const unsigned char *)aptr;
    const unsigned char *b = (const unsigned char *)bptr;
    for (uint32_t i = 0; i < size; i++)
    {
        if (a[i] < b[i])
            return -1;
        else if (b[i] < a[i])
            return 1;
    }
    return 0;
}

void *memset(void *bufptr, int value, uint32_t size)
{
    unsigned char *buf = (unsigned char *)bufptr;
    for (uint32_t i = 0; i < size; i++)
        buf[i] = (unsigned char)value;
    return bufptr;
}

void *memcpy(void *restrict dstptr, const void *restrict srcptr, uint32_t size)
{
    unsigned char *dst = (unsigned char *)dstptr;
    const unsigned char *src = (const unsigned char *)srcptr;
    for (uint32_t i = 0; i < size; i++)
        dst[i] = src[i];
    return dstptr;
}