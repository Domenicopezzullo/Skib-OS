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
  draw_time(x, y, fg, bg);
}

#endif

#endif
