format ELF

extrn keyboard_handler
extrn mouse_handler
public keyboard_handler_wrapper
public mouse_handler_wrapper
public timer_handler_wrapper

section '.text' executable

mouse_handler_wrapper:
  pushad
  cld
  call mouse_handler
  popad
  iretd

timer_handler_wrapper:
  pushad
  mov al,0x20
  out 0x20,al
  popad
  iretd

keyboard_handler_wrapper:
  pushad
  cld
  call keyboard_handler
  popad
  iretd
