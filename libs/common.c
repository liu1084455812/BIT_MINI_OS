// <宋傲>

#include "common.h"

// 端口写一个字节
inline void outb(uint16_t port, uint8_t value)
{
	//内联汇编模板 a:用eax暂存mapped value  dN: edx暂存 N代表port可以用64bit伪指令移动
	//https://gcc.gnu.org/onlinedocs/gcc-7.1.0/gcc/Extended-Asm.html#Assembler-Template
	asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

//读端口字节
inline uint8_t inb(uint16_t port)
{
	uint8_t ret;

	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));

	return ret;
}

//读端口字
inline uint16_t inw(uint16_t port)
{
	uint16_t ret;

	asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));

	return ret;
}

// 开启中断
inline void enable_intr()
{
    asm volatile ("sti");
}

// 关闭中断
inline void disable_intr()
{
    asm volatile ("cli" ::: "memory");
}

// </宋傲>
