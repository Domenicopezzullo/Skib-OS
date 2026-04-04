#define VGA_IMPL
#define UI_IMPL
#define LIBSKIB_IMPL
#define FONT_IMPL
#define MEM_IMPL
#define KEYBOARD_IMPL
#define IDT_IMPL

extern unsigned int keyboard_handler_wrapper; 


#include "drivers/idt.h"
#include "drivers/keyboard.h"
#include "drivers/ui.h"

__attribute__((section(".text.entry")))
void _start() {
    clear_screen(0x001C6BA8);
    pic_remap();
    set_idt_gate(33, keyboard_handler_wrapper);
    load_idt();
    draw_rect(0, SCREEN_HEIGHT - 40, SCREEN_WIDTH, 40, 0x00D4D0C8);
    draw_hline(0, SCREEN_HEIGHT - 40, SCREEN_WIDTH, 0x00FFFFFF);
    __asm__ volatile ("sti");
    int x_finder = 0; 
    while(1) {
        update_clock(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 27, 0x00000000, 0x00D4D0C8);
        for(volatile int i = 0; i < 2000000; ++i);
    }
}
