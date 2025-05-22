#include "stringcustom.h"
char* strcpy(char* dest, const char* src)
{
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}
char* strcat(char* dest, const char* src)
{
    while (*dest != '\0') {
        dest++;
    }
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}
size_t strlen(const char* str)
{
    int length = 0;
    while (*str != '\0') {
        length++;
        str++;
    }
    return length;
}
int strcmp(const char* str1, const char* str2)
{
    while (*str1 && *str2) {
        if (*str1 != *str2) {
            return (unsigned char)(*str1) - (unsigned char)(*str2);
        }
        str1++;
        str2++;
    }
    return (unsigned char)(*str1) - (unsigned char)(*str2);
}

int strncmp(const char* str1, const char* str2, size_t n)
{
    while (n--) {
        if (*str1 != *str2) {
            return (unsigned char)(*str1) - (unsigned char)(*str2);
        }
        if (*str1 == '\0') {
            break;
        }
        str1++;
        str2++;
    }
    return 0;
}