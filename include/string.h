// <宋傲>

//一些库函数的声明和实现
#ifndef INCLUDE_STRING_H_
#define INCLUDE_STRING_H_

#include "types.h"

static inline void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len)
{
	__asm__ ("cld\n\t"
             "rep\n\t"
             "movsb"
             ::"c" (len),"S" (src),"D" (dest)
            );
}

static inline void memset(void *dest, uint8_t val, uint32_t len)
{
	__asm__("cld\n\t"
            "rep\n\t"
            "stosb"
            ::"a" (val),"D" (dest),"c" (len)
            );
}

static inline void bzero(void *dest, uint32_t len)
{
	memset(dest, 0, len);
}

static inline int strcmp(const char *str1, const char *str2)
{
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }

    return *str1 - *str2;
}

static inline char *strcpy(char *dest, const char *src)
{
	__asm__("cld\n"
            "1:\tlodsb\n\t"
            "stosb\n\t"
            "testb %%al,%%al\n\t"
            "jne 1b"
            ::"S" (src),"D" (dest));
    return dest;
}

static inline char *strcat(char *dest, const char *src)
{
	__asm__("cld\n\t"
            "repne\n\t"
            "scasb\n\t"
            "decl %1\n"
            "1:\tlodsb\n\t"
            "stosb\n\t"
            "testb %%al,%%al\n\t"
            "jne 1b"
            ::"S" (src),"D" (dest),"a" (0),"c" (0xffffffff));
    return dest;
}

static inline int strlen(const char *src)
{
	register int __res ;
    __asm__("cld\n\t"
            "repne\n\t"
            "scasb\n\t"
            "notl %0\n\t"
            "decl %0"
            :"=c" (__res):"D" (src),"a" (0),"0" (0xffffffff));
    return __res;
}

#endif 	// INCLUDE_STRING_H_

// </宋傲>

