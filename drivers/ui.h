#ifndef UI_H
#define UI_H

#include "vga.h"

void update_clock(int x, int y, unsigned int fg, unsigned int bg);
void draw_taskbar(unsigned int bg);

#ifdef UI_IMPL

void draw_taskbar(unsigned int bg) {
  draw_rect(0, SCREEN_HEIGHT - 30, SCREEN_WIDTH, 30, bg);
  draw_hline(0, SCREEN_HEIGHT - 30, SCREEN_WIDTH, 0x00FFFFFF);
}

static int last_s = -1;
void update_clock(int x, int y, unsigned int fg, unsigned int bg) {
  unsigned char reg0 = get_rtc_register(0x00);
  int s = bcd_to_bin(reg0);

  if (s != last_s && s < 60) {
    draw_rect(x - 2, y - 2, 140, 20, bg);
    draw_time(x, y, fg, bg);
    last_s = s;
  }
}

#endif

#endif
