#define VGA_IMPL
#define UI_IMPL
#define LIBSKIB_IMPL
#define FONT_IMPL
#define MEM_IMPL
#define KEYBOARD_IMPL
#define MOUSE_IMPL
#define IDT_IMPL

extern void keyboard_handler_wrapper(void);
extern void mouse_handler_wrapper(void);
extern void timer_handler_wrapper(void);

#include "drivers/idt.h"
// needed for keyboard_handler
#include "drivers/keyboard.h" // IWYU pragma: keep
#include "drivers/mouse.h"
#include "drivers/ui.h"

void init_state() {
  pic_remap();
  set_idt_gate(32, (unsigned int)timer_handler_wrapper);
  set_idt_gate(33, (unsigned int)keyboard_handler_wrapper);
  set_idt_gate(44, (unsigned int)mouse_handler_wrapper);
  load_idt();

  mouse_install();
}

__attribute__((section(".text.entry"))) void _start() {
  clear_screen(0x001C6BA8);
  init_state();
  draw_rect(0, SCREEN_HEIGHT - 40, SCREEN_WIDTH, 40, 0x00D4D0C8);
  draw_hline(0, SCREEN_HEIGHT - 40, SCREEN_WIDTH, 0x00FFFFFF);
  __asm__ volatile("sti");
  while (1) {
    update_clock(SCREEN_WIDTH - 150, SCREEN_HEIGHT - 27, 0x00000000,
                 0x00D4D0C8);
    for (volatile int i = 0; i < 2000000; ++i)
      ;
  }
}
