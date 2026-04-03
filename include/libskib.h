#ifndef LIBSKIB_H
#define LIBSKIB_H

extern int cursor;
void update_hardware_cursor();

char* itoa(unsigned int val, int base, char* buf); 
int strncmp(const char* s1, const char* s2, int n);
int strcmp(char* s1, char* s2);
int strlen(const char* s);

#ifdef LIBSKIB_IMPL
char* itoa(unsigned int val, int base, char* buf) {
    
    char digits[] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
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


int strncmp(const char* s1, const char* s2, int n) {
  while(n && *s1 && (*s1 == *s2)) {
    s1++;
    s2++;
    n--;
  }
  if(n == 0) return 0;
  return *(unsigned char*)s1 - *(unsigned char*)s2;
}

int strlen(const char* s) {
  int len = 0;
  while(s[len]) len++;
  return len;
}

int strcmp(char* s1, char* s2) {
  while(*s1 && (*s1 == *s2)) {
    s1++;
    s2++;
  }
  return *(unsigned char*)s1 - *(unsigned char*)s2;
}
#endif
#endif

