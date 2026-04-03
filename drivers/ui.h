#ifndef UI_H
#define UI_H

#include "vga.h"

void update_clock(int x, int y, unsigned char fg, unsigned char bg);

#ifdef UI_IMPL

static int last_s = -1;

void update_clock(int x, int y, unsigned char fg, unsigned char bg) {
    int s = bcd_to_bin(get_rtc_register(0x00));
    
    if (s != last_s) {
        draw_time(x, y, fg, bg); 
        last_s = s;
    }
}

#endif

#endif
