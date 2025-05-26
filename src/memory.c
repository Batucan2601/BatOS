#include "memory.h"
void* memcpy(void* dest, const void* src, uint32_t n) {
    unsigned char* d = (unsigned char* )dest;
    const unsigned char* s = (unsigned char*)src;
    for (uint32_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

void memset(void* dest, uint8_t val, uint32_t n) {
    unsigned char* d = (unsigned char*)dest;
    for (uint32_t i = 0; i < n; i++) {
        d[i] = (unsigned char)val;
    }
}
