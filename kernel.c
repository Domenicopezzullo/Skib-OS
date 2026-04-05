#define VGA_IMPL
#define UI_IMPL
#define LIBSKIB_IMPL
#define FONT_IMPL
#define MEM_IMPL
#define KEYBOARD_IMPL
#define MOUSE_IMPL
#define IDT_IMPL

int mouse_x = 400, mouse_y = 300;

extern void keyboard_handler_wrapper(void);
extern void mouse_handler_wrapper(void);
extern void timer_handler_wrapper(void);

#include "drivers/idt.h"
// needed for keyboard_handler
#include "drivers/keyboard.h" // IWYU pragma: keep
#include "drivers/mouse.h"
#include "drivers/ui.h"
unsigned int fb_addr;
unsigned int fb_pitch;

void init_state() {
  fb_addr = *(unsigned int *)0x0500;
  fb_pitch = *(unsigned int *)0x0504;
  vga_init();
  pic_remap();
  set_idt_gate(32, (unsigned int)timer_handler_wrapper);
  set_idt_gate(33, (unsigned int)keyboard_handler_wrapper);
  set_idt_gate(44, (unsigned int)mouse_handler_wrapper);
  load_idt();

  mouse_install();
}

__attribute__((section(".text.entry"))) void _start() {
  init_state();
  __asm__ volatile("sti");
  while (1) {
    clear_screen(0x001C6BA8);
    draw_rect(0, SCREEN_HEIGHT - 40, SCREEN_WIDTH, 40, 0x00D4D0C8);
    draw_hline(0, SCREEN_HEIGHT - 40, SCREEN_WIDTH, 0x00FFFFFF);
    update_clock(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 27, 0x00000000,
                 0x00D4D0C8);

    extern int mouse_x, mouse_y;
    draw_mouse_cursor(mouse_x, mouse_y);

    vga_flip();
  }
}
