format ELF

extrn keyboard_handler
public keyboard_handler_wrapper

section '.text' executable

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
