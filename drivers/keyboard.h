#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "io.h"
#include "vga.h"
#define KBD_DATA_PORT 0x60
#define KBD_STATUS_PORT 0x64
#define KBD_STATUS_MASK 0x01

static inline unsigned char read_scancode() {
    while(!(inb(KBD_STATUS_PORT) & KBD_STATUS_MASK));
    return inb(KBD_DATA_PORT);
}

__attribute__((used))
void keyboard_handler();

static char scancode_to_char[] = {
0,  27, '1', '2', '3', '4', '5', '6', '7', '8',
'9', '0', '-', '=', '\b',
'\t',
'q', 'w', 'e', 'r',
't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
0,
'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
'\'', '`',   0,
'\\', 'z', 'x', 'c', 'v', 'b', 'n',
'm', ',', '.', '/',   0,
'*',
0,
' ',
};

#ifdef KEYBOARD_IMPL

__attribute__((used))
void keyboard_handler() {
    put_pixel(5,5,0x0000FF00);
    unsigned char status = inb(0x64);
    if (status & 0x01) {
        unsigned char scancode = inb(0x60);
        if (scancode < 0x80) {
            draw_char(0,0,'*',0x0000FF00,0x001C6BA8, 2);
        }
    }
    outb(0x20,0x20);
}
#endif
#endif
