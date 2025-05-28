#pragma once
#include <stdint.h>
void* memcpy(void* dest, const void* src, uint32_t n);
void memset(void* dest, uint8_t val, uint32_t n);
int memcmp(const void* s1, const void* s2, uint32_t n);