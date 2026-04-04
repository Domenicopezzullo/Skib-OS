#ifndef MEM_H
#define MEM_H

#include "io.h"

unsigned short get_total_memory();

#ifdef MEM_IMPL

unsigned short get_total_memory() {
  unsigned short low, high;

  outb(0x70, 0x17);
  low = inb(0x71);
  outb(0x70, 0x18);
  high = inb(0x71);

  return (high << 8) | low;
}

#endif
#endif
