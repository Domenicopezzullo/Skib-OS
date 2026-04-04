#ifndef RTC_H
#define RTC_H

#include "io.h"
#define CMOS_ADDR 0x70
#define CMOS_DATA 0x71

static inline int is_update_in_progress() {
  outb(CMOS_ADDR, 0x0A);
  return (inb(CMOS_DATA) & 0x80);
}

static inline int is_bcd() {
  outb(0x70, 0x0B);
  return !(inb(0x71) & 0x04);
}

static inline int format_rtc_val(unsigned char val) {
  if (is_bcd())
    return ((val / 16) * 10) + (val % 16);
  return val;
}

static inline unsigned char get_rtc_register(int reg) {
  while (is_update_in_progress())
    ;
  outb(CMOS_ADDR, reg);
  return inb(CMOS_DATA);
}

static inline int bcd_to_bin(unsigned char bcd) { return format_rtc_val(bcd); }

#endif
