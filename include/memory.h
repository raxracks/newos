#pragma once
#include <stdint.h>

void *memset(void *dest, int c, uint32_t n);
void *memcpy(void *dest, const void *src, uint32_t n);
void *memmove(void *dest, const void *src, uint32_t n);