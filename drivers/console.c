// <宋傲>

#include "console.h"
#include "common.h"
#include "vmm.h"

static uint16_t *vga_buffer = (uint16_t *)(0xB8000 + PAGE_OFFSET);

//屏幕光标XY
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

//更新光标位置
static void move_cursor()
{
	//屏幕尺寸80*25
	uint16_t cursor_location = cursor_y*80 + cursor_x;

	//http://web.stanford.edu/class/cs140/projects/pintos/specs/freevga/vga/crtcreg.htm#0A
	//读取内部寄存器索引3D4，写入下一步要操作的寄存器编号。之后读取3D5
	outb(0x3D4,14); //14号寄存器是光标位置高8位
	outb(0x3D5,cursor_location>>8);
	outb(0x3D4,15); //15号为低8位；
	outb(0x3D5,cursor_location);
}

//屏幕向上滚动一行
static void scroll()
{
	uint8_t console_color_attribute = (0<<4)|(15&0x0F); //黑底白字
	uint16_t whitespace = (0x20)|(console_color_attribute<<8);

	//下一行复制到上一行
	if(cursor_y>=25)
	{
		for(int i=0;i<80*24;i++)
		{
			vga_buffer[i] = vga_buffer[i+80];
		}
		//最后一行填空格
		for(int i=24*80;i<25*80;i++)
		{
			vga_buffer[i] = whitespace;
		}

		cursor_y = 24;
	}
	
}

//清屏幕
void console_clear()
{
	uint8_t console_color_attribute = (0<<4) | (15&0x0F); //黑底白字
	uint16_t whitespace = (0x20) | (console_color_attribute<<8);

	//将这个屏幕的显示置成whitespace
	for(int i = 0;i<80*25;i++)
	{
		vga_buffer[i] = whitespace;
	}

	cursor_y = cursor_x = 0;
	move_cursor();
}

// putchar
void console_putc_color(char c, real_color_t back, real_color_t fore)
{
	uint8_t console_color_attribute = (((uint8_t)back)<<4) | (((uint8_t)fore)&0x0F);

	//0x08--退格
	if(c == 0x08 && cursor_x)
	{
		cursor_x--;
	}
	//回车
	else if(c == '\r')
	{
		cursor_x = 0;
	}
	//换行
	else if(c =='\n')
	{
		cursor_x = 0;
		cursor_y++;
	}
	else if(c>=' ')
	{
		vga_buffer[cursor_y*80+cursor_x] = (console_color_attribute<<8) | c ;
		cursor_x++;
	}

	//一行满后跳至下一行
	if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y ++;
    }

	//如果屏幕满，向下滚动一行
	scroll();

	//更新光标位置
	move_cursor();
}

// 输出字符串
void console_write(char *cstr)
{
	while (*cstr) {
	      console_putc_color(*cstr++, rc_black, rc_white);
	}
}

// 输出彩色字符串
void console_write_color(char *cstr, real_color_t back, real_color_t fore)
{
	while (*cstr) {
	      console_putc_color(*cstr++, back, fore);
	}
}

// 十六进制
void console_write_hex(uint32_t n, real_color_t back, real_color_t fore)
{
	int tmp;
	char noZeroes = 1;

	console_write_color("0x", back, fore);

	int i;
	for (i = 28; i >= 0; i -= 4) {
		tmp = (n >> i) & 0xF;
		if (tmp == 0 && noZeroes != 0) {
		      continue;
		}
		noZeroes = 0;
		if (tmp >= 0xA) {
		      console_putc_color(tmp-0xA+'a', back, fore);
		} else {
		      console_putc_color(tmp+'0', back, fore);
		}
	}
}

// 十进制
void console_write_dec(uint32_t n, real_color_t back, real_color_t fore)
{
	if (n == 0) {
		console_putc_color('0', back, fore);
		return;
	}

	uint32_t acc = n;
	char c[32];
	int i = 0;
	while (acc > 0) {
		c[i] = '0' + acc % 10;
		acc /= 10;
		i++;
	}
	c[i] = 0;

	char c2[32];
	c2[i--] = 0;

	int j = 0;
	while(i >= 0) {
	      c2[i--] = c[j++];
	}

	console_write_color(c2, back, fore);
}
// </宋傲>

