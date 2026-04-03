org 0x7C00
use16

start:
    
    cli                 
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00      
    
    
    mov [boot_drive], dl

    
    mov ax, 0x0013
    int 0x10

    
    xor ax, ax
    mov es, ax          
    mov bx, 0x1000      

    mov ah, 0x02        
    mov al, 15         
    mov ch, 0           
    mov dh, 0           
    mov cl, 2           
    mov dl, 0
    int 0x13
    jc disk_error       

    
    in al, 0x92
    or al, 2
    out 0x92, al

    
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1           
    mov cr0, eax

    
    jmp 0x08:pmode

disk_error:
    hlt
    jmp $

use32
pmode:
    
    mov ax, 0x10        
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    
    mov esp, 0x90000    

    
    jmp 0x08:0x1000


align 16
gdt:
  dq 0 
    
  
  dw 0xFFFF, 0x0000
  db 0, 10011010b, 11001111b, 0
    
  
  dw 0xFFFF, 0x0000
  db 0, 10010010b, 11001111b, 0
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt - 1
    dd gdt

boot_drive: db 0        

times 510-($-$$) db 0
dw 0xAA55
