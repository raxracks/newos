#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <vga_text.h>
#include <io.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

unsigned char vgat_fg = 0xf;
unsigned char vgat_bg = 0x0;
int vgat_x = 0;
int vgat_y = 0;
bool vgat_cursor_follow = true;

void VGAT_PUTC(unsigned char c, int x, int y, unsigned char fg, unsigned char bg)
{
	uint16_t colors = (bg << 4) | (fg & 0x0F);
	*((volatile uint16_t *)0xB8000 + (y * 80 + x)) = c | (colors << 8);
}

void vgat_putc(unsigned char c)
{
	if (c == '\n')
	{
		vgat_x = 0;
		vgat_y++;
		if (vgat_y >= VGA_HEIGHT)
			vgat_scroll();
		return;
	}

	VGAT_PUTC(c, vgat_x, vgat_y, vgat_fg, vgat_bg);

	vgat_x++;

	if (vgat_cursor_follow)
		vgat_cursor_update(vgat_x, vgat_y);
}

void vgat_print(const char *string)
{
	while (*string != 0)
	{
		vgat_putc(*string++);
	}
}

void vgat_fill(unsigned char color)
{
	for (int x = 0; x < VGA_WIDTH; x++)
		for (int y = 0; y < VGA_HEIGHT; y++)
			VGAT_PUTC(' ', x, y, color, color);
}

void vgat_rect(int x, int y, int w, int h, unsigned char color)
{
	for (int vx = x; vx < x + w; vx++)
		for (int vy = y; vy < y + h; vy++)
			VGAT_PUTC(' ', vx, vy, color, color);
}

void vgat_scroll()
{
	memcpy(0xB8000, 0xB8000 + VGA_WIDTH * 2, (VGA_WIDTH * VGA_HEIGHT) * 2);

	vgat_rect(0, VGA_HEIGHT - 1, VGA_WIDTH, 1, vgat_bg);

	vgat_y--;
}

void vgat_printf(const char *str, ...)
{
	va_list list;
	va_start(list, 256);

	for (int i = 0; str[i] != '\0'; i++)
	{
		switch (str[i])
		{
		case '%':
			i++;

			if (str[i] == 's')
				vgat_print(va_arg(list, const char *));
			if (str[i] == 'd')
				vgat_print(itoa(va_arg(list, int), NULL, 10));
			if (str[i] == 'x')
				vgat_print(itoa(va_arg(list, int), NULL, 16));
			break;
		default:
			vgat_putc(str[i]);
			break;
		}
	}

	va_end(list);
}

void vgat_cursor_disable()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void vgat_cursor_enable(uint8_t cursor_start, uint8_t cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void vgat_cursor_update(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

uint16_t vgat_cursor_position()
{
	uint16_t pos = 0;
	outb(0x3D4, 0x0F);
	pos |= inb(0x3D5);
	outb(0x3D4, 0x0E);
	pos |= ((uint16_t)inb(0x3D5)) << 8;
	return pos;
}