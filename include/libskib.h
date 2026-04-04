#ifndef LIBSKIB_H
#define LIBSKIB_H

#include "../drivers/mem.h"

extern int cursor;
void update_hardware_cursor();

void *malloc(unsigned int size);
char *itoa(unsigned int val, int base, char *buf);
int strncmp(const char *s1, const char *s2, int n);
int strcmp(char *s1, char *s2);
int strlen(const char *s);

#ifdef LIBSKIB_IMPL

static char *heap_ptr = (char *)0x200000;
static unsigned int total_memory = 0;

void *malloc(unsigned int size) {
  if (size % 8 != 0)
    size += (8 - (size % 8));

  char *next_ptr = heap_ptr + size;
  if (total_memory == 0) {
    total_memory = (unsigned int)get_total_memory() * 1024;
    if (total_memory == 0)
      total_memory = 32 * 1024 * 1024;
  }

  if ((unsigned int)next_ptr > total_memory) {
    return 0;
  }

  void *res = (void *)heap_ptr;
  heap_ptr = next_ptr;
  return res;
}

char *itoa(unsigned int val, int base, char *buf) {

  char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                   '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  char temp[32];
  int i = 0;

  if (val == 0) {
    buf[0] = '0';
    buf[1] = '\0';
    return buf;
  }

  while (val > 0) {
    temp[i++] = digits[val % base];
    val /= base;
  }

  int j = 0;
  while (i > 0) {
    buf[j++] = temp[--i];
  }
  buf[j] = '\0';

  return buf;
}

int strncmp(const char *s1, const char *s2, int n) {
  while (n && *s1 && (*s1 == *s2)) {
    s1++;
    s2++;
    n--;
  }
  if (n == 0)
    return 0;
  return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int strlen(const char *s) {
  int len = 0;
  while (s[len])
    len++;
  return len;
}

int strcmp(char *s1, char *s2) {
  while (*s1 && (*s1 == *s2)) {
    s1++;
    s2++;
  }
  return *(unsigned char *)s1 - *(unsigned char *)s2;
}
#endif
#endif
