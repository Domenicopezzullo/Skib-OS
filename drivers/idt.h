#ifndef IDT_H
#define IDT_H

struct idt_entry {
    unsigned short base_low;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short base_high;
} __attribute__((packed));

struct idt_ptr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

void set_idt_gate(int n, unsigned int handler);
void load_idt();

#ifdef IDT_IMPL

struct idt_entry idt[256];
struct idt_ptr idtp;

void set_idt_gate(int n, unsigned int handler) {
    idt[n].base_low = handler & 0xFFFF;
    idt[n].sel = 0x08; 
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; 
    idt[n].base_high = (handler >> 16) & 0xFFFF;
}

void load_idt() {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base  = (unsigned int)&idt;

    
    __asm__ volatile("lidt (%0)" : : "r" (&idtp));
}

#endif

#endif
