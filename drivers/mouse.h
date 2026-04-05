#ifndef MOUSE_H
#define MOUSE_H

#include "io.h"
#include "vga.h"

#define MOUSE_DATA_PORT 0x60
#define MOUSE_STATUS_PORT 0x64
#define MOUSE_COMMAND_PORT 0x64

static unsigned int cursor_backup[64] = {0};
static int cursor_initialized = 0;

void mouse_wait(unsigned char type);
void mouse_write(unsigned char write);
void mouse_install();
void mouse_handler();
unsigned char mouse_read();

#ifdef MOUSE_IMPL

extern int mouse_x, mouse_y;
static unsigned char mouse_state = 0;
static signed char mouse_dx = 0, mouse_dy = 0;

unsigned char mouse_read() {
  unsigned int timeout = 100000;
  while (timeout--) {
    if ((inb(0x64) & 1) == 1) {
      return inb(0x60);
    }
  }
  return 0;
}

unsigned char mouse_cycle = 0;
unsigned char mouse_byte[3];
void mouse_handler() {
  unsigned char data = inb(MOUSE_DATA_PORT);

  if (mouse_state == 0) {
    if (!(data & 0x08))
      return;

    mouse_byte[0] = data;
    mouse_state = 1;
  } else if (mouse_state == 1) {
    mouse_byte[1] = data;
    mouse_state = 2;
  } else if (mouse_state == 2) {
    mouse_byte[2] = data;
    mouse_state = 0;

    mouse_x += (signed char)mouse_byte[1];
    mouse_y -= (signed char)mouse_byte[2];
    if (mouse_x < 0)
      mouse_x = 0;
    if (mouse_x > SCREEN_WIDTH - 8)
      mouse_x = SCREEN_WIDTH - 8;
    if (mouse_y < 0)
      mouse_y = 0;
    if (mouse_y > SCREEN_HEIGHT - 8)
      mouse_y = SCREEN_HEIGHT - 8;
    if (mouse_x < 0)
      mouse_x = 0;
    if (mouse_x > SCREEN_WIDTH - 8)
      mouse_x = SCREEN_WIDTH - 8;
    if (mouse_y < 0)
      mouse_y = 0;
    if (mouse_y > SCREEN_HEIGHT - 8)
      mouse_y = SCREEN_HEIGHT - 8;
  }
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
      if ((inb(MOUSE_STATUS_PORT) & 1) == 1)
        return;
    }
  } else {
    while (timeout--) {
      if ((inb(MOUSE_STATUS_PORT) & 2) == 0)
        return;
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
