// <宋傲>

#include "console.h"
#include "string.h"
#include "vargs.h"
#include "debug.h"

static int vsprintf(char *buff, const char *format, va_list args);

void printk(const char *format, ...)
{
	static char buff[1024];
	va_list args;
	int i;

	va_start(args, format);
	i = vsprintf(buff, format, args);
	va_end(args);

	buff[i] = '\0';

	console_write(buff);
}

void printk_color(real_color_t back, real_color_t fore, const char *format, ...)
{
	static char buff[1024];
	va_list args;
	int i;

	va_start(args, format);
	i = vsprintf(buff, format, args);
	va_end(args);

	buff[i] = '\0';

	console_write_color(buff, back, fore);
}

// </宋傲>