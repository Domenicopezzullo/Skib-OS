#define VGA_IMPL
#define UI_IMPL
#define LIBSKIB_IMPL
#define FONT_IMPL
#define MEM_IMPL

#include "drivers/ui.h"
#include "drivers/vga.h"
#include "include/libskib.h"

__attribute__((section(".text.entry")))
void _start() {
    clear_screen(0); 

    
    draw_rect(0, 0, 320, 12, 1); 
    draw_string(5, 2, "SKIB-OS KERNEL v0.1", 15, 1);

    
    int win_x = 40, win_y = 40, win_w = 240, win_h = 100;
    
    
    draw_rect(win_x, win_y, win_w, win_h, 8);
    
    draw_rect_outline(win_x, win_y, win_w, win_h, 7);
    
    
    draw_string(win_x + 5, win_y + 5, "System Console", 14, 8);
    draw_hline(win_x, win_y + 15, win_w, 7);

    
    print_hex(win_x + 10, win_y + 30, "Memory Base: 0x", 0x1000, 11, 8);
    print_hex(win_x + 10, win_y + 45, "Stack Top:   0x", 0x90000, 10, 8);
    draw_string(win_x + 10, win_y + 60, "Status:      Running", 15, 8);

    
    draw_rect(0, 188, 320, 12, 7);
    draw_string(5, 190, "START", 0, 7);


    while(1) {
        update_clock(250, 2, 15, 1);
        for(volatile int i = 0; i < 2000000; ++i);
        __asm__ volatile ("hlt");
    }
    while(1) {__asm__ volatile("hlt"); }
}
