#ifndef VGA_H
#define VGA_H

#include "rtc.h"

#include "font.h"

#define LIBSKIB_IMPL
#include "../include/libskib.h"

#define VIDEO_ADDRESS 0xA0000
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 200

void draw_time(int x, int y, unsigned char fg, unsigned char bg);
void print_hex(int x, int y, char* prefix, int val, unsigned char fg, unsigned char bg);
void put_pixel(int x, int y, unsigned char color);
void clear_screen(unsigned char color);
void draw_char(int x, int y, char c, unsigned char fg_color, unsigned char bg_color);
void draw_string(int x, int y, char* str, unsigned char fg, unsigned char bg);
void draw_rect(int x, int y, int w, int h, unsigned char color);
void draw_hline(int x, int y, int w, unsigned char color);
void draw_vline(int x, int y, int h, unsigned char color);
void draw_rect_outline(int x, int y, int w, int h, unsigned char color);

#ifdef VGA_IMPL

void draw_time(int x, int y, unsigned char fg, unsigned char bg) {
    
    unsigned char raw_h = get_rtc_register(0x04);
    unsigned char raw_m = get_rtc_register(0x02);
    unsigned char raw_s = get_rtc_register(0x00);

    
    int h = bcd_to_bin(raw_h);
    int m = bcd_to_bin(raw_m);
    int s = bcd_to_bin(raw_s);

    
    h = (h + 2) % 24; 

    char buf[16];
    int cur_x = x;

    
    if (h < 10) { draw_string(cur_x, y, "0", fg, bg); cur_x += 8; }
    itoa(h, 10, buf);
    draw_string(cur_x, y, buf, fg, bg); 
    
    cur_x = x + 16;
    draw_string(cur_x, y, ":", fg, bg); 
    cur_x += 8;

    
    if (m < 10) { draw_string(cur_x, y, "0", fg, bg); cur_x += 8; }
    itoa(m, 10, buf);
    draw_string(cur_x, y, buf, fg, bg);

    cur_x = x + 40;
    draw_string(cur_x, y, ":", fg, bg);
    cur_x += 8;

    
    if (s < 10) { draw_string(cur_x, y, "0", fg, bg); cur_x += 8; }
    itoa(s, 10, buf);
    draw_string(cur_x, y, buf, fg, bg);
}

void draw_rect(int x, int y, int w, int h, unsigned char color) {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            put_pixel(x + i, y + j, color);
        }
    }
}

void draw_rect_outline(int x, int y, int w, int h, unsigned char color) {
    draw_hline(x, y, w, color);           
    draw_hline(x, y + h - 1, w, color);   
    draw_vline(x, y, h, color);           
    draw_vline(x + w - 1, y, h, color);   
}

void draw_hline(int x, int y, int w, unsigned char color) {
    for (int i = 0; i < w; i++) put_pixel(x + i, y, color);
}

void draw_vline(int x, int y, int h, unsigned char color) {
    for (int i = 0; i < h; i++) put_pixel(x, y + i, color);
}

void print_hex(int x, int y, char* prefix, int val, unsigned char fg, unsigned char bg) {
    char buf[32];
    draw_string(x, y, prefix, fg,bg);
    
    int offset = 0;
    while(prefix[offset] != '\0') offset++;
    
    itoa(val, 16, buf);
    draw_string(x + (offset * 8), y, buf, fg,bg);
}


void draw_char(int x, int y, char c, unsigned char fg_color, unsigned char bg_color) {
    unsigned char *bitmap = font_bitmap[(unsigned char)c];
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (bitmap[row] & (1 << (7 - col))) {
                put_pixel(x + col, y + row, fg_color);
            } else {
                
                put_pixel(x + col, y + row, bg_color); 
            }
        }
    }
}

void draw_string(int x, int y, char* str,unsigned char fg, unsigned char bg) {
    for (int i = 0; str[i] != '\0'; i++) {
        draw_char(x + (i * 8), y, str[i], fg, bg);
    }
}

void put_pixel(int x, int y, unsigned char color) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) return;
    volatile unsigned char *vga = (unsigned char*)VIDEO_ADDRESS;
    vga[y * SCREEN_WIDTH + x] = color;
}

void clear_screen(unsigned char color) {
    volatile unsigned char *vga = (unsigned char*)VIDEO_ADDRESS;
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        vga[i] = color;
    }
}

#endif 
#endif 
