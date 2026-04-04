#ifndef MOUSE_H
#define MOUSE_H

#include "vga.h"
#include "io.h"


#define MOUSE_DATA_PORT 0x60
#define MOUSE_STATUS_PORT 0x64
#define MOUSE_COMMAND_PORT 0x64

static unsigned int cursor_backup[64] = {0};
static int cursor_initialized = 0;

static unsigned char cursor_sprite[8] = {
    0x80,
    0xC0,
    0xE0,
    0xF0,
    0xF8,
    0xFC,
    0xFE,
    0x00
};void mouse_wait(unsigned char type);
void mouse_write(unsigned char write);
void mouse_install();
void mouse_handler();
unsigned char mouse_read();

#ifdef MOUSE_IMPL

static int mouse_x = 400, mouse_y = 300;
static unsigned char mouse_state = 0;
static signed char mouse_dx=0,mouse_dy=0;

unsigned char mouse_read() {
    unsigned int timeout = 100000;
    while (timeout--) {
        if ((inb(0x64) & 1) == 1){ 
            return inb(0x60);
      }
    }
    return 0;
}

unsigned char mouse_cycle = 0;
unsigned char mouse_byte[3];
void mouse_handler() {
    unsigned char status = inb(MOUSE_STATUS_PORT);
    if((status & 0x01) == 0) { outb(0xA0, 0x20); outb(0x20,0x20); return; }
    unsigned char data = inb(MOUSE_DATA_PORT);
    if(mouse_state == 0 && (data & 0x08)) {
        mouse_state = 1;
        mouse_dx = 0;
        mouse_dy = 0;
    } else if (mouse_state == 1) {
        mouse_dx = (signed char)data;
        mouse_state = 2;
    } else if (mouse_state == 2) {
        mouse_dy = -((signed char)data);
        mouse_state = 0;

        int old_x = mouse_x;
        int old_y = mouse_y;

        mouse_x += mouse_dx;
        mouse_y += mouse_dy;

        
        if(mouse_x < 0) mouse_x = 0;
        if(mouse_x > SCREEN_WIDTH - 8) mouse_x = SCREEN_WIDTH - 8;
        if(mouse_y < 0) mouse_y = 0;
        if(mouse_y > SCREEN_HEIGHT - 8) mouse_y = SCREEN_HEIGHT - 8;

        unsigned int* fb = (unsigned int*)(*(unsigned int*)0x0500);
        unsigned int pitch_px = (*(unsigned int*)0x0504) / 4;

        
        if (cursor_initialized) {
            for(int i = 0; i < 8; i++) {
                for(int j = 0; j < 8; j++) {
                    fb[(old_y + i) * pitch_px + (old_x + j)] = cursor_backup[i * 8 + j];
                }
            }
        }

        
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                cursor_backup[i * 8 + j] = fb[(mouse_y + i) * pitch_px + (mouse_x + j)];
            }
        }

        
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                
                if(cursor_sprite[i] & (1 << (7 - j))) {
                    fb[(mouse_y + i) * pitch_px + (mouse_x + j)] = 0x00FFFFFF; 
                }
            }
        }

        cursor_initialized = 1;
    }
    outb(0xA0, 0x20);
    outb(0x20,0x20);
}


void mouse_install() {
    unsigned char status;

    mouse_wait(1);
    outb(0x64, 0xA8);

    mouse_wait(1);
    outb(0x64, 0x20);
    mouse_wait(0);
    status = (inb(0x60) | 2);
    mouse_wait(1);
    outb(0x64, 0x60);
    mouse_wait(1);
    outb(0x60, status);

    mouse_write(0xF6);
    mouse_read(); 

    mouse_write(0xF4);
    mouse_read(); 
}


void mouse_wait(unsigned char type) {
    unsigned int timeout = 100000;
    if (type == 0) { 
        while (timeout--) {
            if ((inb(MOUSE_STATUS_PORT) & 1) == 1) return;
        }
    } else { 
        while (timeout--) {
            if ((inb(MOUSE_STATUS_PORT) & 2) == 0) return;
        }
    }
}

void mouse_write(unsigned char write) {
    mouse_wait(1);
    outb(MOUSE_COMMAND_PORT, 0xD4);
    mouse_wait(1);
    outb(MOUSE_DATA_PORT, write);
}

#endif
#endif
