#ifndef VGA_H
#define VGA_H

#include "rtc.h"

#include "font.h"

#include "../include/libskib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

static inline unsigned int get_pitch() { return *(unsigned int *)0x0504; }

void vga_init();
void vga_flip();
void draw_mouse_cursor(int x, int y);
void draw_time(int x, int y, unsigned int fg, unsigned int bg);
void print_hex(int x, int y, char *prefix, int val, unsigned int fg,
               unsigned int bg, int size);
void put_pixel(int x, int y, unsigned int color);
void clear_screen(unsigned int color);
void draw_char(int x, int y, char c, unsigned int fg_color,
               unsigned int bg_color, int size);
void draw_string(int x, int y, char *str, unsigned int fg, unsigned int bg,
                 int size);
void draw_rect(int x, int y, int w, int h, unsigned int color);
void draw_hline(int x, int y, int w, unsigned int color);
void draw_vline(int x, int y, int h, unsigned int color);
void draw_rect_outline(int x, int y, int w, int h, unsigned int color);
#ifdef VGA_IMPL

void draw_mouse_cursor(int x, int y) {
  unsigned char cursor_sprite[8] = {0x80, 0xC0, 0xF0, 0xF8, 0xFC, 0xFE, 0x00};
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (cursor_sprite[i] & (1 << (7 - j))) {
        put_pixel(x + j, y + i, 0x00FFFFFF);
      }
    }
  }
}

static unsigned int *back_buffer = 0;
static unsigned int fb_pixels = SCREEN_WIDTH * SCREEN_HEIGHT;
extern unsigned int fb_addr;
extern unsigned int fb_pitch;

void vga_flip() {
  unsigned int *front = (unsigned int *)fb_addr;
  unsigned int pitch_px = fb_pitch / 4;
  unsigned int *bb = back_buffer;
  for (int y = 0; y < SCREEN_HEIGHT; ++y) {
    unsigned int *dst = front + y * pitch_px;
    unsigned int *src = bb + y * SCREEN_WIDTH;
    for (int x = 0; x < SCREEN_WIDTH; x += 4) {
      dst[x] = src[x];
      dst[x + 1] = src[x + 1];
      dst[x + 2] = src[x + 2];
      dst[x + 3] = src[x + 3];
    }
  }
}

void vga_init() {
  back_buffer = (unsigned int *)malloc(fb_pixels * 4);
  if (back_buffer == 0) {
  }
}

void draw_time(int x, int y, unsigned int fg, unsigned int bg) {
  while (is_update_in_progress())
    ;
  unsigned char raw_h = get_rtc_register(0x04);
  unsigned char raw_m = get_rtc_register(0x02);
  unsigned char raw_s = get_rtc_register(0x00);

  int size = 2;
  int char_width = 8 * size;

  int h = bcd_to_bin(raw_h);
  int m = bcd_to_bin(raw_m);
  int s = bcd_to_bin(raw_s);

  h = (h + 2) % 24;

  char clock_str[9];
  clock_str[0] = (h / 10) + '0';
  clock_str[1] = (h % 10) + '0';
  clock_str[2] = ':';
  clock_str[3] = (m / 10) + '0';
  clock_str[4] = (m % 10) + '0';
  clock_str[5] = ':';
  clock_str[6] = (s / 10) + '0';
  clock_str[7] = (s % 10) + '0';
  clock_str[8] = '\0';

  draw_string(x, y, clock_str, fg, bg, size);
}

void draw_rect(int x, int y, int w, int h, unsigned int color) {
  for (int j = 0; j < h; j++) {
    for (int i = 0; i < w; i++) {
      put_pixel(x + i, y + j, color);
    }
  }
}

void draw_rect_outline(int x, int y, int w, int h, unsigned int color) {
  draw_hline(x, y, w, color);
  draw_hline(x, y + h - 1, w, color);
  draw_vline(x, y, h, color);
  draw_vline(x + w - 1, y, h, color);
}

void draw_hline(int x, int y, int w, unsigned int color) {
  for (int i = 0; i < w; i++)
    put_pixel(x + i, y, color);
}

void draw_vline(int x, int y, int h, unsigned int color) {
  for (int i = 0; i < h; i++)
    put_pixel(x, y + i, color);
}

void print_hex(int x, int y, char *prefix, int val, unsigned int fg,
               unsigned int bg, int size) {
  char buf[32];
  draw_string(x, y, prefix, fg, bg, size);

  int offset = 0;
  while (prefix[offset] != '\0')
    offset++;

  itoa(val, 16, buf);
  draw_string(x + (offset * 8 * size), y, buf, fg, bg, size);
}

void draw_char(int x, int y, char c, unsigned int fg, unsigned int bg,
               int size) {
  unsigned char *bitmap = font_bitmap[(unsigned char)c];
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {

      unsigned int color = (bitmap[i] & (1 << (7 - j))) ? fg : bg;

      for (int sy = 0; sy < size; sy++) {
        for (int sx = 0; sx < size; sx++) {
          put_pixel(x + (j * size) + sx, y + (i * size) + sy, color);
        }
      }
    }
  }
}

void draw_string(int x, int y, char *str, unsigned int fg, unsigned int bg,
                 int size) {
  for (int i = 0; str[i] != '\0'; i++) {
    draw_char(x + (i * 8 * size), y, str[i], fg, bg, size);
  }
}

void put_pixel(int x, int y, unsigned int color) {
  if (!back_buffer)
    return;
  if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
    return;

  back_buffer[y * SCREEN_WIDTH + x] = color;
}

void clear_screen(unsigned int color) {
  unsigned int *bb = back_buffer;
  unsigned int count = SCREEN_WIDTH * SCREEN_HEIGHT;
  for (unsigned int i = 0; i < count; ++i) {
    bb[i] = color;
  }
}

#endif
#endif
