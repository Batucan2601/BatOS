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

int memcmp(const void* s1, const void* s2, uint32_t n)
{
    const unsigned char* p1 = (const unsigned char*)s1;
    const unsigned char* p2 = (const unsigned char*)s2;
    for (uint32_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }
    return 0; // They are equal
}
